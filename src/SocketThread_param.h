#ifdef SOCKET_THREAD_PARAM_H
#error Already included
#else
#define SOCKET_THREAD_PARAM_H

struct ThreadParams
{
    HANDLE event;
    HWND hwnd;
    HWND hwndEdit;
};

#endif
