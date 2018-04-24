#include <windows.h>
#include <stdlib.h>
#include <vector>

#include "main.h"
#include "SocketThread_param.h"
#include "SocketThread.h"
#include "Console.h"


#define INP_BUFFER_SIZE (3 * 192000)


static struct GlobalData G =
{
    nullptr,
    INVALID_HANDLE_VALUE,
    { },
    0,
    { },
    nullptr,
    FALSE,
    0
};

Console *console = nullptr;

static std::vector<BYTE> buffer1(INP_BUFFER_SIZE);
static std::vector<BYTE> buffer2(INP_BUFFER_SIZE);
static WAVEHDR WaveHdr1;
static WAVEHDR WaveHdr2;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static HMENU AppCustomMenu(void);

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
    auto hFileMenu = CreateMenu();
    {
        (void)AppendMenu(hFileMenu, MF_STRING, IDM_APP_START, TEXT("Start"));
        (void)AppendMenu(hFileMenu, MF_STRING, IDM_APP_STOP, TEXT("Stop"));
        (void)AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
        (void)AppendMenu(hFileMenu, MF_STRING, IDM_APP_EXIT, TEXT("&Exit"));
    }

    auto hThreadMenu = CreateMenu();
    {
        (void)AppendMenu(hThreadMenu, MF_STRING, IDM_APP_START_SOCKET_THREAD, TEXT("Start"));
        (void)AppendMenu(hThreadMenu, MF_STRING, IDM_APP_STOP_SOCKET_THREAD, TEXT("Stop"));
    }

    auto hMenu = CreateMenu();
    {
        (void)AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("&File"));
        (void)AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hThreadMenu, TEXT("&Socket Thread"));
    }

    return hMenu;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            {
                console = new Console(hwnd, ((LPCREATESTRUCT)lParam)->hInstance);

                if (console->is_error())
                {
                    (void)MessageBox(NULL, TEXT("Error creating console"), TEXT("WndProc"), MB_ICONERROR);
                }

                struct ThreadParams params;
                params.hwnd = hwnd;
                params.hwndEdit = nullptr;
                params.event = CreateEvent(NULL, FALSE, FALSE, NULL);

                G.pSocketThread = new SocketThread { params };
                G.pSocketThread->start();

                return 0;
            }

        case WM_SETFOCUS:
            {
                console->SetFocus();
                return 0;
            }

        case WM_SIZE:
            {
                console->MoveWindow(0, 0, LOWORD(lParam), HIWORD(lParam));
                return 0;
            }

        case WM_USER_START:
            {
                *console << TEXT("Command: Start") << Console::eol;

                G.bStopRecord = FALSE;
                G.dwAudioDataCount = 0;

                G.waveform.wFormatTag = WAVE_FORMAT_PCM;
                G.waveform.nChannels = 1;
                G.waveform.nSamplesPerSec = 192000;
                G.waveform.nAvgBytesPerSec = (3 * 192000);
                G.waveform.nBlockAlign = 3;
                G.waveform.wBitsPerSample = 24;
                G.waveform.cbSize = 0;

                auto waveInOpen_result = waveInOpen(
                            &G.hWaveIn,
                            WAVE_MAPPER,
                            &G.waveform,
                            (DWORD_PTR)hwnd,
                            0,
                            CALLBACK_WINDOW);

                if (MMSYSERR_NOERROR != waveInOpen_result)
                {
                    *console << TEXT("Error: waveInOpen failed") << Console::eol;
                    return 0;
                }

                CloseFileHandle(&G.hFileOut);

                wsprintf(G.szFileName, "audio-%d.raw", G.fileCount);

                G.hFileOut = CreateFile(
                        G.szFileName,
                        GENERIC_WRITE,
                        FILE_SHARE_WRITE,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

                ++G.fileCount;

                if (G.hFileOut == INVALID_HANDLE_VALUE)
                {
                    *console << TEXT("Error: File not created: ") << G.szFileName << Console::eol;
                }

                WaveHdr1.lpData = (LPSTR)buffer1.data();
                WaveHdr1.dwBufferLength = buffer1.size();
                WaveHdr1.dwBytesRecorded = 0;
                WaveHdr1.dwUser = 0;
                WaveHdr1.dwFlags = 0;
                WaveHdr1.dwLoops = 1;
                WaveHdr1.lpNext = NULL;
                WaveHdr1.reserved = 0;
                waveInPrepareHeader(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));

                WaveHdr2.lpData = (LPSTR)buffer2.data();
                WaveHdr2.dwBufferLength = buffer2.size();
                WaveHdr2.dwBytesRecorded = 0;
                WaveHdr2.dwUser = 0;
                WaveHdr2.dwFlags = 0;
                WaveHdr2.dwLoops = 1;
                WaveHdr2.lpNext = NULL;
                WaveHdr2.reserved = 0;
                waveInPrepareHeader(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));

                return 0;
            }

        case WM_USER_STOP:
            {
                *console << TEXT("Command: Stop") << Console::eol;
                G.bStopRecord = TRUE;
                waveInReset(G.hWaveIn);
                return 0;
            }

        case MM_WIM_OPEN:
            {
                waveInAddBuffer(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
                waveInAddBuffer(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
                waveInStart(G.hWaveIn);
                *console << TEXT("Audio input opened") << Console::eol;
                return 0;
            }

        case MM_WIM_DATA:
            {
                PWAVEHDR pWaveHdr = (PWAVEHDR)lParam;

                if (NULL == pWaveHdr)
                {
                    *console << TEXT("Error: pWaveHdr is invalid") << Console::eol;
                    return 0;
                }

                ++G.dwAudioDataCount;
                *console << G.dwAudioDataCount << TEXT(": Audio samples: ") << pWaveHdr->dwBytesRecorded << Console::eol;

                if (0 != pWaveHdr->dwBytesRecorded)
                {
                    if (G.hFileOut != INVALID_HANDLE_VALUE)
                    {
                        DWORD WrittenResult = 0u;
                        WriteFile(G.hFileOut, pWaveHdr->lpData, pWaveHdr->dwBytesRecorded, &WrittenResult, NULL);
                        (void)FlushFileBuffers(G.hFileOut);
                    }
                }

                if (FALSE != G.bStopRecord)
                {
                    waveInClose(G.hWaveIn);
                    return 0;
                }

                waveInAddBuffer(G.hWaveIn, pWaveHdr, sizeof(WAVEHDR));

                return 0;
            }

        case MM_WIM_CLOSE:
            {
                waveInUnprepareHeader(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
                waveInUnprepareHeader(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
                *console << TEXT("Audio input closed") << Console::eol;
                CloseFileHandle(&G.hFileOut);
                return 0;
            }

        case WM_COMMAND:
            {
                auto cmd = LOWORD(wParam);

                if (IDM_APP_START == cmd)
                {
                    *console << TEXT("Menu command: Start") << Console::eol;
                    PostMessage(hwnd, WM_USER_START, 0, 0);
                }

                if (IDM_APP_STOP == cmd)
                {
                    *console << TEXT("Menu command: Stop") << Console::eol;
                    PostMessage(hwnd, WM_USER_STOP, 0, 0);
                }

                if (IDM_APP_START_SOCKET_THREAD == cmd)
                {
                    *console << TEXT("Menu command: Start socket thread") << Console::eol;
                }

                if (IDM_APP_STOP_SOCKET_THREAD == cmd)
                {
                    *console << TEXT("Menu command: Stop socket thread") << Console::eol;
                }

                if (IDM_APP_EXIT == cmd)
                {
                    *console << TEXT("Menu command: Exit") << Console::eol;
                    G.bStopRecord = TRUE;
                    waveInReset(G.hWaveIn);
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                }

                return 0;
            }

        case WM_DESTROY:
            {
                G.bStopRecord = TRUE;
                waveInReset(G.hWaveIn);
                CloseFileHandle(&G.hFileOut);
                delete G.pSocketThread;
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
