#ifdef ADDR_INFO_H
#error Already included
#else
#define ADDR_INFO_H

struct addrinfo;

class AddrInfo
{
    public:
        explicit AddrInfo(const std::string &port);
        ~AddrInfo();

        bool is_error() const;

        struct addrinfo *get() const;

    private:
        bool m_error;
        struct addrinfo *p_info;

    public:
        AddrInfo() = delete;
        AddrInfo(const AddrInfo &instance) = delete;
        AddrInfo(const AddrInfo &&instance) = delete;
        AddrInfo &operator=(const AddrInfo &instance) = delete;
        AddrInfo &operator=(const AddrInfo &&instance) = delete;
};

#endif
