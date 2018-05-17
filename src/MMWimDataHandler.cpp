#include <Windows.h>

#include "WndProcHandler.h"
#include "MMWimDataHandler.h"
#include "main.h"
#include "Console.h"


MM_WIM_DATA_Handler::MM_WIM_DATA_Handler() :
    WndProcHandler {}
{
}

MM_WIM_DATA_Handler::~MM_WIM_DATA_Handler()
{
}

LRESULT MM_WIM_DATA_Handler::operator()([[maybe_unused]] HWND hwnd, [[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
    PWAVEHDR pWaveHdr = (PWAVEHDR)lParam;

    if (NULL == pWaveHdr)
    {
        *console << TEXT("Error: pWaveHdr is invalid") << Console::eol;
        return 0;
    }

    ++G.dwAudioDataCount;
    *console << G.dwAudioDataCount << TEXT(": Audio samples: ") << pWaveHdr->dwBytesRecorded << Console::eol;

    if (0 != pWaveHdr->dwBytesRecorded)
    {
        if (G.hFileOut != INVALID_HANDLE_VALUE)
        {
            DWORD WrittenResult = 0u;
            WriteFile(G.hFileOut, pWaveHdr->lpData, pWaveHdr->dwBytesRecorded, &WrittenResult, NULL);
            (void)FlushFileBuffers(G.hFileOut);
        }
    }

    if (FALSE != G.bStopRecord)
    {
        waveInClose(G.hWaveIn);
        return 0;
    }

    waveInAddBuffer(G.hWaveIn, pWaveHdr, sizeof(WAVEHDR));

    return 0;
}
