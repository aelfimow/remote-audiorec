#include <Windows.h>

#include "WndProcHandler.h"
#include "WMDestroyHandler.h"
#include "main.h"
#include "Console.h"
#include "SocketThread.h"


WMDestroyHandler::WMDestroyHandler() :
    WndProcHandler {}
{
}

WMDestroyHandler::~WMDestroyHandler()
{
}

LRESULT WMDestroyHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    hwnd = hwnd;
    wParam = wParam;
    lParam = lParam;

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
