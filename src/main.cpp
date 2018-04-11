#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <vector>

#include "main.h"
#include "SocketThread_param.h"


#define ID_EDIT         1
#define IDM_APP_EXIT    40000
#define IDM_APP_START   40001
#define IDM_APP_STOP    40002

#define MY_PORT         "50000"
#define RX_BUFFER_SIZE  1024

#define INP_BUFFER_SIZE (3 * 192000)


void EditPrintf(HWND hwndEdit, const TCHAR *szFormat, ...)
{
    std::vector<TCHAR> szBuffer(1024);
    va_list pArgList;

    va_start(pArgList, szFormat);
    wvsprintf(&szBuffer[0], szFormat, pArgList);
    va_end(pArgList);

    if (hwndEdit == NULL)
    {
        return;
    }

    SendMessage(hwndEdit, EM_SETSEL, (WPARAM)-1, (WPARAM)-1);
    SendMessage(hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)szBuffer.data());
    SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);

    TCHAR szEndLine[] = TEXT("\r\n");
    SendMessage(hwndEdit, EM_SETSEL, (WPARAM)-1, (WPARAM)-1);
    SendMessage(hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)&szEndLine[0]);
    SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);
}


static std::vector<BYTE> buffer1(INP_BUFFER_SIZE);
static std::vector<BYTE> buffer2(INP_BUFFER_SIZE);
static WAVEHDR WaveHdr1;
static WAVEHDR WaveHdr2;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static HMENU AppCustomMenu(void);

static DWORD WINAPI SockThread(LPVOID pvoid);
static void CloseFileHandle(HANDLE *pHandle);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static const TCHAR szAppName[] = TEXT("remote-audiorec");
    WNDCLASS wndclass;

    hPrevInstance = hPrevInstance;
    szCmdLine = szCmdLine;

    wndclass.style = (CS_HREDRAW | CS_VREDRAW);
    wndclass.lpfnWndProc = &WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = &szAppName[0];

    if (0 == RegisterClass(&wndclass))
    {
        (void)MessageBox(NULL, TEXT("Error in RegisterClass."), &szAppName[0], MB_ICONERROR);
        return 0;
    }

    HWND hwnd = CreateWindow(
            &szAppName[0],
            &szAppName[0],
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            NULL,
            AppCustomMenu(),
            hInstance,
            NULL);

    if (NULL == hwnd)
    {
        (void)MessageBox(NULL, TEXT("Error in CreateWindow."), &szAppName[0], MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, iCmdShow);

    if (FALSE == UpdateWindow(hwnd))
    {
        (void)MessageBox(NULL, TEXT("Error in UpdateWindow."), &szAppName[0], MB_ICONERROR);
        return 0;
    }

    MSG msg;
    auto msgres = GetMessage(&msg, NULL, 0, 0);

    while ((msgres != 0) && (msgres != -1))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        msgres = GetMessage(&msg, NULL, 0, 0);
    }

    if (msgres == -1)
    {
        (void)MessageBox(NULL, TEXT("Error in GetMessage."), &szAppName[0], MB_ICONERROR);
        return 0;
    }

    return 0;
}

static HMENU AppCustomMenu(void)
{
    HMENU hMenu = CreateMenu();
    HMENU hMenuPopup = CreateMenu();

    (void)AppendMenu(hMenuPopup, MF_STRING, IDM_APP_START, TEXT("Start"));
    (void)AppendMenu(hMenuPopup, MF_STRING, IDM_APP_STOP, TEXT("Stop"));
    (void)AppendMenu(hMenuPopup, MF_SEPARATOR, 0, NULL);
    (void)AppendMenu(hMenuPopup, MF_STRING, IDM_APP_EXIT, TEXT("&Exit"));

    (void)AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hMenuPopup, TEXT("&File"));

    return hMenu;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static DWORD SockThreadID = 0;
    static HANDLE hSockThread = NULL;
    static HWND hwndEdit = NULL;
    static HANDLE hFileOut = INVALID_HANDLE_VALUE;
    static TCHAR szFileName[512];
    static DWORD fileCount = 0;

    static WAVEFORMATEX waveform;
    static HWAVEIN hWaveIn = NULL;
    static BOOL bStopRecord = FALSE;
    static DWORD dwAudioDataCount = 0;

    switch (message)
    {
        case WM_CREATE:
            {
                hwndEdit = CreateWindow(
                        TEXT("edit"),
                        NULL,
                        (WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | WS_BORDER | \
                         ES_READONLY | ES_LEFT | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL),
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)ID_EDIT,
                        ((LPCREATESTRUCT)lParam)->hInstance,
                        NULL);

                if (NULL == hwndEdit)
                {
                    (void)MessageBox(NULL, TEXT("Error: hwndEdit == NULL."), TEXT("WndProc"), MB_ICONERROR);
                }

                // Set limit of text to max.
                SendMessage(hwndEdit, EM_LIMITTEXT, 0, 0L);

                struct ThreadParams params;
                params.hwnd = hwnd;
                params.hwndEdit = hwndEdit;
                params.event = CreateEvent(NULL, FALSE, FALSE, NULL);

                hSockThread = CreateThread(NULL, 0, &SockThread, (LPVOID)&params, 0, &SockThreadID);

                if (NULL != hSockThread)
                {
                    /* thread created, wait for thread creating message queue */
                    WaitForSingleObject(params.event, INFINITE);
                }

                return 0;
            }

        case WM_SETFOCUS:
            {
                if (NULL != hwndEdit)
                {
                    SetFocus(hwndEdit);
                }
                return 0;
            }

        case WM_SIZE:
            {
                if (NULL != hwndEdit)
                {
                    MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
                }
                return 0;
            }

        case WM_USER_START:
            {
                EditPrintf(hwndEdit, TEXT("Command: Start"));

                bStopRecord = FALSE;
                dwAudioDataCount = 0;

                waveform.wFormatTag = WAVE_FORMAT_PCM;
                waveform.nChannels = 1;
                waveform.nSamplesPerSec = 192000;
                waveform.nAvgBytesPerSec = (3 * 192000);
                waveform.nBlockAlign = 3;
                waveform.wBitsPerSample = 24;
                waveform.cbSize = 0;

                if (MMSYSERR_NOERROR != waveInOpen(
                            &hWaveIn,
                            WAVE_MAPPER,
                            &waveform,
                            (DWORD_PTR)hwnd,
                            0,
                            CALLBACK_WINDOW))
                {
                    EditPrintf(hwndEdit, TEXT("Error: waveInOpen failed"));
                    return 0;
                }

                CloseFileHandle(&hFileOut);

                wsprintf(szFileName, "audio-%d.raw", fileCount);

                hFileOut = CreateFile(
                        szFileName,
                        GENERIC_WRITE,
                        FILE_SHARE_WRITE,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

                ++fileCount;

                if (hFileOut == INVALID_HANDLE_VALUE)
                {
                    EditPrintf(hwndEdit, TEXT("Error: File not created: %s"), szFileName);
                }

                WaveHdr1.lpData = (LPSTR)buffer1.data();
                WaveHdr1.dwBufferLength = buffer1.size();
                WaveHdr1.dwBytesRecorded = 0;
                WaveHdr1.dwUser = 0;
                WaveHdr1.dwFlags = 0;
                WaveHdr1.dwLoops = 1;
                WaveHdr1.lpNext = NULL;
                WaveHdr1.reserved = 0;
                waveInPrepareHeader(hWaveIn, &WaveHdr1, sizeof(WAVEHDR));

                WaveHdr2.lpData = (LPSTR)buffer2.data();
                WaveHdr2.dwBufferLength = buffer2.size();
                WaveHdr2.dwBytesRecorded = 0;
                WaveHdr2.dwUser = 0;
                WaveHdr2.dwFlags = 0;
                WaveHdr2.dwLoops = 1;
                WaveHdr2.lpNext = NULL;
                WaveHdr2.reserved = 0;
                waveInPrepareHeader(hWaveIn, &WaveHdr2, sizeof(WAVEHDR));

                return 0;
            }

        case WM_USER_STOP:
            {
                EditPrintf(hwndEdit, TEXT("Command: Stop"));
                bStopRecord = TRUE;
                waveInReset(hWaveIn);
                return 0;
            }

        case MM_WIM_OPEN:
            {
                waveInAddBuffer(hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
                waveInAddBuffer(hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
                waveInStart(hWaveIn);
                EditPrintf(hwndEdit, TEXT("Audio input opened"));
                return 0;
            }

        case MM_WIM_DATA:
            {
                PWAVEHDR pWaveHdr = (PWAVEHDR)lParam;

                if (NULL == pWaveHdr)
                {
                    EditPrintf(hwndEdit, TEXT("Error: pWaveHdr is invalid"));
                    return 0;
                }

                ++dwAudioDataCount;
                EditPrintf(hwndEdit, TEXT("%d: Audio samples: %d"), dwAudioDataCount, pWaveHdr->dwBytesRecorded);

                if (0 != pWaveHdr->dwBytesRecorded)
                {
                    if (hFileOut != INVALID_HANDLE_VALUE)
                    {
                        DWORD WrittenResult = 0u;
                        WriteFile(hFileOut, pWaveHdr->lpData, pWaveHdr->dwBytesRecorded, &WrittenResult, NULL);
                        (void)FlushFileBuffers(hFileOut);
                    }
                }

                if (FALSE != bStopRecord)
                {
                    waveInClose(hWaveIn);
                    return 0;
                }

                waveInAddBuffer(hWaveIn, pWaveHdr, sizeof(WAVEHDR));

                return 0;
            }

        case MM_WIM_CLOSE:
            {
                waveInUnprepareHeader(hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
                waveInUnprepareHeader(hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
                EditPrintf(hwndEdit, TEXT("Audio input closed"));
                CloseFileHandle(&hFileOut);
                return 0;
            }

        case WM_COMMAND:
            {
                /* check for errors from edit field */
                if (ID_EDIT == LOWORD(wParam))
                {
                    if ((EN_ERRSPACE == HIWORD(wParam)) || (EN_MAXTEXT == HIWORD(wParam)))
                    {
                        /* edit window has no RAM and reports error */
                        /* do nothing here */
                    }
                }

                if (IDM_APP_START == LOWORD(wParam))
                {
                    EditPrintf(hwndEdit, TEXT("Menu command: Start"));
                    PostMessage(hwnd, WM_USER_START, 0, 0);
                }

                if (IDM_APP_STOP == LOWORD(wParam))
                {
                    EditPrintf(hwndEdit, TEXT("Menu command: Stop"));
                    PostMessage(hwnd, WM_USER_STOP, 0, 0);
                }

                if (IDM_APP_EXIT == LOWORD(wParam))
                {
                    EditPrintf(hwndEdit, TEXT("Menu command: Exit"));
                    bStopRecord = TRUE;
                    waveInReset(hWaveIn);
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                }

                return 0;
            }

        case WM_DESTROY:
            {
                bStopRecord = TRUE;
                waveInReset(hWaveIn);
                CloseFileHandle(&hFileOut);
                PostQuitMessage(0);
                return 0;
            }

        default:
            {
                /* no action here */
                break;
            }
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

static DWORD WINAPI SockThread(LPVOID pvoid)
{
    struct ThreadParams params;
    MSG msg;

    memcpy(&params, pvoid, sizeof(struct ThreadParams));

    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    SetEvent(params.event);

    EditPrintf(params.hwndEdit, TEXT("Thread started"));

    while (1)
    {
        EditPrintf(params.hwndEdit, TEXT("Waiting for commands"));

        WSADATA wsaData;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        char recvbuf[RX_BUFFER_SIZE];

        int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

        if (iResult != 0)
        {
            EditPrintf(params.hwndEdit, TEXT("WSAStartup failed with error: %d\n"), iResult);
            break;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        iResult = getaddrinfo(NULL, MY_PORT, &hints, &result);

        if (iResult != 0)
        {
            EditPrintf(params.hwndEdit, TEXT("getaddrinfo failed with error: %d\n"), iResult);
            WSACleanup();
            break;
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            EditPrintf(params.hwndEdit, TEXT("socket failed with error: %ld\n"), WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            break;
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(params.hwndEdit, TEXT("bind failed with error: %d\n"), WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(params.hwndEdit, TEXT("listen failed with error: %d\n"), WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);

        if (ClientSocket == INVALID_SOCKET)
        {
            EditPrintf(params.hwndEdit, TEXT("accept failed with error: %d\n"), WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        closesocket(ListenSocket);

        do
        {
            ZeroMemory(recvbuf, sizeof(recvbuf));
            iResult = recv(ClientSocket, recvbuf, sizeof(recvbuf) - 1, 0);

            if (iResult > 0)
            {
                EditPrintf(params.hwndEdit, TEXT("Bytes received %d, message: %s\n"), iResult, recvbuf);

                if (0 == strcmp(recvbuf, "start"))
                {
                    (void)PostMessage(params.hwnd, WM_USER_START, 0, 0);
                }

                if (0 == strcmp(recvbuf, "stop"))
                {
                    (void)PostMessage(params.hwnd, WM_USER_STOP, 0, 0);
                }
            }

            if (iResult < 0)
            {
                EditPrintf(params.hwndEdit, TEXT("recv failed with error: %d\n"), WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                break;
            }

        } while (iResult > 0);

        iResult = shutdown(ClientSocket, SD_SEND);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(params.hwndEdit, TEXT("shutdown failed with error: %d\n"), WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            break;
        }

        closesocket(ClientSocket);
        WSACleanup();
    }

    EditPrintf(params.hwndEdit, TEXT("Thread exit"));

    ExitThread(0);

    return 0;
}

static void CloseFileHandle(HANDLE *pHandle)
{
    if (pHandle != NULL)
    {
        if (*pHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(*pHandle);
            *pHandle = INVALID_HANDLE_VALUE;
        }
    }
}
