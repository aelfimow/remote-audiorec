#include <Windows.h>
#include <vector>

#include "WndProcHandler.h"
#include "WMUserStartHandler.h"
#include "MMWimOpenHandler.h"
#include "main.h"
#include "Console.h"


MMWimOpenHandler::MMWimOpenHandler(WMUserStartHandler &userStartHandler) :
    WndProcHandler {},
    m_UserStartHandler { userStartHandler }
{
}

MMWimOpenHandler::~MMWimOpenHandler()
{
}

LRESULT MMWimOpenHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    hwnd = hwnd;
    wParam = wParam;
    lParam = lParam;

    m_UserStartHandler.addAudioBuffer();

    waveInStart(G.hWaveIn);

    *console << TEXT("Audio input opened") << Console::eol;

    return 0;
}
