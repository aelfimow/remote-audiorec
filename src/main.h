#ifdef MAIN_H
#error Already included
#else
#define MAIN_H

#define ID_EDIT         1

#define IDM_APP_EXIT                    40000
#define IDM_APP_START                   40001
#define IDM_APP_STOP                    40002
#define IDM_APP_START_SOCKET_THREAD     40003
#define IDM_APP_STOP_SOCKET_THREAD      40004

#define WM_USER_START   (WM_USER + 1u)
#define WM_USER_STOP    (WM_USER + 2u)

#define WM_START_SOCKET_THREAD  (WM_USER + 3u)
#define WM_STOP_SOCKET_THREAD   (WM_USER + 4u)

class Console;
class SocketThread;

extern Console *console;

struct GlobalData
{
    SocketThread *pSocketThread;
    HANDLE hFileOut;
    TCHAR szFileName[512];
    DWORD fileCount;
    WAVEFORMATEX waveform;
    HWAVEIN hWaveIn;
    BOOL bStopRecord;
    DWORD dwAudioDataCount;
};

#endif
