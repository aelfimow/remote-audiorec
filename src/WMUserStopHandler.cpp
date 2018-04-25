#include <Windows.h>

#include "WndProcHandler.h"
#include "WMUserStopHandler.h"
#include "main.h"
#include "Console.h"


WMUserStopHandler::WMUserStopHandler() :
    WndProcHandler {}
{
}

WMUserStopHandler::~WMUserStopHandler()
{
}

LRESULT WMUserStopHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    hwnd = hwnd;
    wParam = wParam;
    lParam = lParam;

    *console << TEXT("Command: Stop") << Console::eol;
    G.bStopRecord = TRUE;
    waveInReset(G.hWaveIn);

    return 0;
}
