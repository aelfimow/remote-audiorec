#ifdef SOCKET_THREAD_PRIV_H
#error Already included
#else
#define SOCKET_THREAD_PRIV_H

struct ThreadParams;

class SocketThread_priv
{
    public:
        explicit SocketThread_priv(const struct ThreadParams &params);
        ~SocketThread_priv();

        void start();

    private:
        HANDLE hThread;
        HANDLE hEvent;
        HWND hwnd;
        HWND hwndEdit;
        DWORD threadID;

        static DWORD WINAPI threadFunc(LPVOID pvoid);

    public:
        SocketThread_priv() = delete;
        SocketThread_priv(const SocketThread_priv &instance) = delete;
        SocketThread_priv(const SocketThread_priv &&instance) = delete;
        SocketThread_priv &operator=(const SocketThread_priv &instance) = delete;
        SocketThread_priv &operator=(const SocketThread_priv &&instance) = delete;
};

#endif
