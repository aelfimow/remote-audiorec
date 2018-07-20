#include <Windows.h>

#include "WndProcParam.h"
#include "WndProcHandler.h"
#include "WM_DESTROY_Handler.h"
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

LRESULT WM_DESTROY_Handler::operator()([[maybe_unused]] const WndProcParam &param)
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
