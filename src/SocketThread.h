#ifdef SOCKET_THREAD_H
#error Already included
#else
#define SOCKET_THREAD_H

struct ThreadParams;
class SocketThread_priv;

class SocketThread
{
    public:
        explicit SocketThread(const struct ThreadParams &params);
        ~SocketThread();

        void start();

    private:
        SocketThread_priv * const m_priv;

    public:
        SocketThread() = delete;
        SocketThread(const SocketThread &instance) = delete;
        SocketThread(const SocketThread &&instance) = delete;
        SocketThread &operator=(const SocketThread &instance) = delete;
        SocketThread &operator=(const SocketThread &&instance) = delete;
};

#endif
