#include <Windows.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include "main.h"
#include "SocketThread_param.h"
#include "SocketThread.h"
#include "Console.h"
#include "WndProcHandler.h"
#include "WMSizeHandler.h"
#include "WMSetFocusHandler.h"
#include "WMCreateHandler.h"
#include "WMUserStartHandler.h"
#include "WMUserStopHandler.h"
#include "MMWimOpenHandler.h"
#include "MMWimDataHandler.h"
#include "MMWimCloseHandler.h"
#include "WMCommandHandler.h"
#include "WMDestroyHandler.h"


struct GlobalData G =
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

static WMSizeHandler      WMSize;
static WMSetFocusHandler  WMSetFocus;
static WMCreateHandler    WMCreate;
static WMUserStartHandler WMUserStart;
static WMUserStopHandler  WMUserStop;
static MMWimOpenHandler   MMWimOpen { WMUserStart };
static MMWimDataHandler   MMWimData;
static MMWimCloseHandler  MMWimClose { WMUserStart };
static WMCommandHandler   WMCommand;
static WMDestroyHandler   WMDestroy;

static std::map<UINT, WndProcHandler*> WndProcMap;

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
static HMENU AppCustomMenu(void);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static const TCHAR szAppName[] = TEXT("remote-audiorec");
    WNDCLASS wndclass;

    hPrevInstance = hPrevInstance;
    szCmdLine = szCmdLine;

    WndProcMap[WM_CREATE]     = &WMCreate;
    WndProcMap[WM_SETFOCUS]   = &WMSetFocus;
    WndProcMap[WM_SIZE]       = &WMSize;
    WndProcMap[WM_USER_START] = &WMUserStart;
    WndProcMap[WM_USER_STOP]  = &WMUserStop;
    WndProcMap[MM_WIM_OPEN]   = &MMWimOpen;
    WndProcMap[MM_WIM_DATA]   = &MMWimData;
    WndProcMap[MM_WIM_CLOSE]  = &MMWimClose;
    WndProcMap[WM_COMMAND]    = &WMCommand;
    WndProcMap[WM_DESTROY]    = &WMDestroy;

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
    auto it = WndProcMap.find(message);

    if (it == WndProcMap.end())
    {
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    WndProcHandler &handler { *it->second };

    auto result = handler(hwnd, wParam, lParam);

    return result;
}
