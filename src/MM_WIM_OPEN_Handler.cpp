#include <Windows.h>
#include <vector>

#include "WndProcParam.h"
#include "WndProcHandler.h"
#include "WM_USER_START_Handler.h"
#include "MM_WIM_OPEN_Handler.h"
#include "main.h"
#include "Console.h"


MM_WIM_OPEN_Handler::MM_WIM_OPEN_Handler(WM_USER_START_Handler &userStartHandler) :
    WndProcHandler {},
    m_UserStartHandler { userStartHandler }
{
}

MM_WIM_OPEN_Handler::~MM_WIM_OPEN_Handler()
{
}

LRESULT MM_WIM_OPEN_Handler::operator()([[maybe_unused]] const WndProcParam &param)
{
    m_UserStartHandler.addAudioBuffer();

    waveInStart(G.hWaveIn);

    *console << TEXT("Audio input opened") << Console::eol;

    return 0;
}
