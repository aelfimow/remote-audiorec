#include <Windows.h>

#include "WndProcHandler.h"
#include "WMCommandHandler.h"
#include "main.h"
#include "Console.h"


WMCommandHandler::WMCommandHandler() :
    WndProcHandler {}
{
}

WMCommandHandler::~WMCommandHandler()
{
}

LRESULT WMCommandHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    lParam = lParam;

    auto cmd = LOWORD(wParam);

    if (IDM_APP_START == cmd)
    {
        *console << TEXT("Menu command: Start") << Console::eol;
        PostMessage(hwnd, WM_USER_START, 0, 0);
        return 0;
    }

    if (IDM_APP_STOP == cmd)
    {
        *console << TEXT("Menu command: Stop") << Console::eol;
        PostMessage(hwnd, WM_USER_STOP, 0, 0);
        return 0;
    }

    if (IDM_APP_START_SOCKET_THREAD == cmd)
    {
        *console << TEXT("Menu command: Start socket thread") << Console::eol;
        return 0;
    }

    if (IDM_APP_STOP_SOCKET_THREAD == cmd)
    {
        *console << TEXT("Menu command: Stop socket thread") << Console::eol;
        return 0;
    }

    if (IDM_APP_EXIT == cmd)
    {
        *console << TEXT("Menu command: Exit") << Console::eol;
        G.bStopRecord = TRUE;
        waveInReset(G.hWaveIn);
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        return 0;
    }

    *console << TEXT("Menu command: Unknown") << Console::eol;

    return 0;
}