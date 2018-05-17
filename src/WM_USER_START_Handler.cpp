#include <Windows.h>
#include <vector>

#include "main.h"
#include "WndProcHandler.h"
#include "WM_USER_START_Handler.h"
#include "Console.h"


WM_USER_START_Handler::WM_USER_START_Handler() :
    WndProcHandler {},
    buffer1(INP_BUFFER_SIZE),
    buffer2(INP_BUFFER_SIZE),
    WaveHdr1 {},
    WaveHdr2 {}
{
}

WM_USER_START_Handler::~WM_USER_START_Handler()
{
}

LRESULT WM_USER_START_Handler::operator()(HWND hwnd, [[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
    *console << TEXT("Command: Start") << Console::eol;

    G.bStopRecord = FALSE;
    G.dwAudioDataCount = 0;

    G.waveform.wFormatTag = WAVE_FORMAT_PCM;
    G.waveform.nChannels = 1;
    G.waveform.nSamplesPerSec = 192000;
    G.waveform.nAvgBytesPerSec = (3 * 192000);
    G.waveform.nBlockAlign = 3;
    G.waveform.wBitsPerSample = 24;
    G.waveform.cbSize = 0;

    auto waveInOpen_result = waveInOpen(
            &G.hWaveIn,
            WAVE_MAPPER,
            &G.waveform,
            (DWORD_PTR)hwnd,
            0,
            CALLBACK_WINDOW);

    if (MMSYSERR_NOERROR != waveInOpen_result)
    {
        *console << TEXT("Error: waveInOpen failed") << Console::eol;
        return 0;
    }

    if (G.hFileOut != INVALID_HANDLE_VALUE)
    {
        CloseHandle(G.hFileOut);
        G.hFileOut = INVALID_HANDLE_VALUE;
    }

    wsprintf(G.szFileName, "audio-%d.raw", G.fileCount);

    G.hFileOut = CreateFile(
            G.szFileName,
            GENERIC_WRITE,
            FILE_SHARE_WRITE,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

    ++G.fileCount;

    if (G.hFileOut == INVALID_HANDLE_VALUE)
    {
        *console << TEXT("Error: File not created: ") << G.szFileName << Console::eol;
    }

    WaveHdr1.lpData = (LPSTR)buffer1.data();
    WaveHdr1.dwBufferLength = buffer1.size();
    WaveHdr1.dwBytesRecorded = 0;
    WaveHdr1.dwUser = 0;
    WaveHdr1.dwFlags = 0;
    WaveHdr1.dwLoops = 1;
    WaveHdr1.lpNext = NULL;
    WaveHdr1.reserved = 0;
    waveInPrepareHeader(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));

    WaveHdr2.lpData = (LPSTR)buffer2.data();
    WaveHdr2.dwBufferLength = buffer2.size();
    WaveHdr2.dwBytesRecorded = 0;
    WaveHdr2.dwUser = 0;
    WaveHdr2.dwFlags = 0;
    WaveHdr2.dwLoops = 1;
    WaveHdr2.lpNext = NULL;
    WaveHdr2.reserved = 0;
    waveInPrepareHeader(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));

    return 0;
}

void WM_USER_START_Handler::addAudioBuffer()
{
    waveInAddBuffer(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
    waveInAddBuffer(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
}

void WM_USER_START_Handler::removeAudioBuffer()
{
    waveInUnprepareHeader(G.hWaveIn, &WaveHdr1, sizeof(WAVEHDR));
    waveInUnprepareHeader(G.hWaveIn, &WaveHdr2, sizeof(WAVEHDR));
}
