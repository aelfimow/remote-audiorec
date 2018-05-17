#include <Windows.h>

#include "WndProcHandler.h"
#include "WMDestroyHandler.h"
#include "main.h"
#include "Console.h"
#include "SocketThread.h"


WM_DESTROY_Handler::WM_DESTROY_Handler() :
    WndProcHandler {}
{
}

WM_DESTROY_Handler::~WM_DESTROY_Handler()
{
}

LRESULT WM_DESTROY_Handler::operator()([[maybe_unused]] HWND hwnd, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
    G.bStopRecord = TRUE;
    waveInReset(G.hWaveIn);

    if (G.hFileOut != INVALID_HANDLE_VALUE)
    {
        CloseHandle(G.hFileOut);
        G.hFileOut = INVALID_HANDLE_VALUE;
    }

    delete G.pSocketThread;

    PostQuitMessage(0);

    return 0;
}
