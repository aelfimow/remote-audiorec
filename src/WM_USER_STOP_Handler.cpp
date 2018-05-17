#include <Windows.h>

#include "WndProcHandler.h"
#include "WM_USER_STOP_Handler.h"
#include "main.h"
#include "Console.h"


WM_USER_STOP_Handler::WM_USER_STOP_Handler() :
    WndProcHandler {}
{
}

WM_USER_STOP_Handler::~WM_USER_STOP_Handler()
{
}

LRESULT WM_USER_STOP_Handler::operator()([[maybe_unused]] HWND hwnd, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
    *console << TEXT("Command: Stop") << Console::eol;
    G.bStopRecord = TRUE;
    waveInReset(G.hWaveIn);

    return 0;
}
