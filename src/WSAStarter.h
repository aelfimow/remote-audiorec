#ifdef WSA_STARTER_H
#error Already included
#else
#define WSA_STARTER_H

class WSAStarter
{
    public:
        WSAStarter();
        ~WSAStarter();

        bool is_error() const;

    private:
        WSADATA m_data;
        bool m_error;

    public:
        WSAStarter(const WSAStarter &instance) = delete;
        WSAStarter(const WSAStarter &&instance) = delete;
        WSAStarter &operator=(const WSAStarter &instance) = delete;
        WSAStarter &operator=(const WSAStarter &&instance) = delete;
};

#endif
