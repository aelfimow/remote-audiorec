#include <Windows.h>

#include "WndProcParam.h"
#include "WndProcHandler.h"
#include "WM_COMMAND_Handler.h"
#include "main.h"
#include "Console.h"


WM_COMMAND_Handler::WM_COMMAND_Handler() :
    WndProcHandler {}
{
}

WM_COMMAND_Handler::~WM_COMMAND_Handler()
{
}

LRESULT WM_COMMAND_Handler::operator()(const WndProcParam &param)
{
    auto cmd = LOWORD(param.wParam());

    if (IDM_APP_START == cmd)
    {
        *console << TEXT("Menu command: Start") << Console::eol;
        PostMessage(param.hwnd(), WM_USER_START, 0, 0);
        return 0;
    }

    if (IDM_APP_STOP == cmd)
    {
        *console << TEXT("Menu command: Stop") << Console::eol;
        PostMessage(param.hwnd(), WM_USER_STOP, 0, 0);
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
        PostMessage(param.hwnd(), WM_CLOSE, 0, 0);
        return 0;
    }

    return 0;
}
