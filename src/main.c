#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#define ID_EDIT         1
#define IDM_APP_EXIT    40000

#define MY_PORT         "50000"
#define RX_BUFFER_SIZE  1024

struct tThreadParams
{
    HANDLE event;
    HWND hwnd;
    HWND hwndEdit;
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static HMENU AppCustomMenu(void);

static DWORD WINAPI SockThread(LPVOID pvoid);
static void EditPrintf(HWND hwndEdit, TCHAR *szFormat, ...);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static const TCHAR szAppName[] = TEXT("remote-audiorec");
    HWND hwnd = NULL;
    MSG msg;
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

    hwnd = CreateWindow(
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

    while (0 != GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

static HMENU AppCustomMenu(void)
{
    HMENU hMenu = CreateMenu();
    HMENU hMenuPopup = CreateMenu();

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
    struct tThreadParams ThreadParams;

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

                ThreadParams.hwnd = hwnd;
                ThreadParams.hwndEdit = hwndEdit;
                ThreadParams.event = CreateEvent(NULL, FALSE, FALSE, NULL);

                hSockThread = CreateThread(NULL, 0, &SockThread, (LPVOID)&ThreadParams, 0, &SockThreadID);

                if (NULL != hSockThread)
                {
                    /* thread created, wait for thread creating message queue */
                    WaitForSingleObject(ThreadParams.event, INFINITE);
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

                if (IDM_APP_EXIT == LOWORD(wParam))
                {
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                }

                return 0;
            }

        case WM_DESTROY:
            {
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
    struct tThreadParams params;
    MSG msg;

    memcpy(&params, pvoid, sizeof(struct tThreadParams));

    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

    SetEvent(params.event);

    EditPrintf(params.hwndEdit, "Thread started");

    while (1)
    {
        EditPrintf(params.hwndEdit, "Waiting for commands");

        WSADATA wsaData;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        char recvbuf[RX_BUFFER_SIZE];

        int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

        if (iResult != 0)
        {
            EditPrintf(params.hwndEdit, "WSAStartup failed with error: %d\n", iResult);
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
            EditPrintf(params.hwndEdit, "getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            break;
        }

        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET)
        {
            EditPrintf(params.hwndEdit, "socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            break;
        }

        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(params.hwndEdit, "bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(params.hwndEdit, "listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            break;
        }

        ClientSocket = accept(ListenSocket, NULL, NULL);

        if (ClientSocket == INVALID_SOCKET)
        {
            EditPrintf(params.hwndEdit, "accept failed with error: %d\n", WSAGetLastError());
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
                EditPrintf(params.hwndEdit, "Bytes received %d, message: %s\n", iResult, recvbuf);
            }

            if (iResult < 0)
            {
                EditPrintf(params.hwndEdit, "recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                break;
            }

        } while (iResult > 0);

        iResult = shutdown(ClientSocket, SD_SEND);

        if (iResult == SOCKET_ERROR)
        {
            EditPrintf(params.hwndEdit, "shutdown failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            break;
        }

        closesocket(ClientSocket);
        WSACleanup();
    }

    EditPrintf(params.hwndEdit, "Thread exit");

    ExitThread(0);

    return 0;
}

static void EditPrintf(HWND hwndEdit, TCHAR *szFormat, ...)
{
    static TCHAR szEndLine[] = TEXT("\r\n");
    TCHAR szBuffer[1024u];
    va_list pArgList;

    memset(&szBuffer[0], 0, sizeof(szBuffer));

    if (NULL != hwndEdit)
    {
        va_start(pArgList, szFormat);
        wvsprintf(&szBuffer[0], szFormat, pArgList);
        va_end(pArgList);

        SendMessage(hwndEdit, EM_SETSEL, (WPARAM)-1, (WPARAM)-1);
        SendMessage(hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)&szBuffer[0]);
        SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);

        SendMessage(hwndEdit, EM_SETSEL, (WPARAM)-1, (WPARAM)-1);
        SendMessage(hwndEdit, EM_REPLACESEL, FALSE, (LPARAM)&szEndLine[0]);
        SendMessage(hwndEdit, EM_SCROLLCARET, 0, 0);
    }
}

