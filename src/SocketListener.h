#ifdef SOCKET_LISTENER_H
#error Already included
#else
#define SOCKET_LISTENER_H

class AddrInfo;

class SocketListener
{
    public:
        explicit SocketListener(const AddrInfo &ai);
        ~SocketListener();

        bool is_error() const;

        void listen();
        void close();

    private:
        bool m_error;
        SOCKET m_socket;

    public:
        SocketListener() = delete;
        SocketListener(const SocketListener &instance) = delete;
        SocketListener(const SocketListener &&instance) = delete;
        SocketListener &operator=(const SocketListener &instance) = delete;
        SocketListener &operator=(const SocketListener &&instance) = delete;
};

#endif
