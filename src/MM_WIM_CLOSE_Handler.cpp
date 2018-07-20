#include <Windows.h>
#include <vector>

#include "WndProcParam.h"
#include "WndProcHandler.h"
#include "WM_USER_START_Handler.h"
#include "MM_WIM_CLOSE_Handler.h"
#include "main.h"
#include "Console.h"


MM_WIM_CLOSE_Handler::MM_WIM_CLOSE_Handler(WM_USER_START_Handler &userStartHandler) :
    WndProcHandler {},
    m_UserStartHandler { userStartHandler }
{
}

MM_WIM_CLOSE_Handler::~MM_WIM_CLOSE_Handler()
{
}

LRESULT MM_WIM_CLOSE_Handler::operator()([[maybe_unused]] const WndProcParam &param)
{
    m_UserStartHandler.removeAudioBuffer();

    *console << TEXT("Audio input closed") << Console::eol;

    if (G.hFileOut != INVALID_HANDLE_VALUE)
    {
        CloseHandle(G.hFileOut);
        G.hFileOut = INVALID_HANDLE_VALUE;
    }

    return 0;
}
