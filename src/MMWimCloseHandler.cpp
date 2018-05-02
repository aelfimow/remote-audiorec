#include <Windows.h>
#include <vector>

#include "WndProcHandler.h"
#include "WMUserStartHandler.h"
#include "MMWimCloseHandler.h"
#include "main.h"
#include "Console.h"


MMWimCloseHandler::MMWimCloseHandler(WMUserStartHandler &userStartHandler) :
    WndProcHandler {},
    m_UserStartHandler { userStartHandler }
{
}

MMWimCloseHandler::~MMWimCloseHandler()
{
}

LRESULT MMWimCloseHandler::operator()(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    hwnd = hwnd;
    wParam = wParam;
    lParam = lParam;

    m_UserStartHandler.removeAudioBuffer();

    *console << TEXT("Audio input closed") << Console::eol;

    if (G.hFileOut != INVALID_HANDLE_VALUE)
    {
        CloseHandle(G.hFileOut);
        G.hFileOut = INVALID_HANDLE_VALUE;
    }

    return 0;
}
