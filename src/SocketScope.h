#ifdef SOCKET_SCOPE_H
#error Already included
#else
#define SOCKET_SCOPE_H

class SocketScope
{
    public:
        explicit SocketScope(const SOCKET s);
        ~SocketScope();

        bool is_error() const;

        std::string recv();

    private:
        const SOCKET m_socket;
        bool m_error;

    public:
        SocketScope() = delete;
        SocketScope(const SocketScope &instance) = delete;
        SocketScope(const SocketScope &&instance) = delete;
        SocketScope &operator=(const SocketScope &instance) = delete;
        SocketScope &operator=(const SocketScope &&instance) = delete;
};

#endif
