#include <ws2tcpip.h>
#include <Windows.h>
#include <string>

#include "AddrInfo.h"


AddrInfo::AddrInfo(const std::string &port) :
    m_error { false },
    p_info { nullptr }
{
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    auto result = getaddrinfo(NULL, port.c_str(), &hints, &p_info);

    m_error = (0 != result);
}

AddrInfo::~AddrInfo()
{
    if (p_info != nullptr)
    {
        freeaddrinfo(p_info);
    }
}

bool AddrInfo::is_error() const
{
    return m_error;
}

struct addrinfo *AddrInfo::get() const
{
    return p_info;
}
