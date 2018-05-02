#include <Windows.h>

#include "WndProcHandler.h"
#include "MMWimOpenHandler.h"
#include "main.h"
#include "Console.h"


MMWimOpenHandler::MMWimOpenHandler() :
    WndProcHandler {}
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

    waveInAddBuffer(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
    waveInAddBuffer(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
    waveInStart(G.hWaveIn);

    *console << TEXT("Audio input opened") << Console::eol;

    return 0;
}
