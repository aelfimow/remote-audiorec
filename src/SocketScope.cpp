#include <winsock2.h>
#include <vector>
#include <string>

#include "SocketScope.h"


SocketScope::SocketScope(const SOCKET s) :
    m_socket { s },
    m_error { false }
{
    m_error = (s == INVALID_SOCKET);
}

SocketScope::~SocketScope()
{
    auto result = shutdown(m_socket, SD_SEND);

    if (result == SOCKET_ERROR)
    {
    }

    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
    }
}

bool SocketScope::is_error() const
{
    return m_error;
}

std::string SocketScope::recv()
{
    std::vector<char> recvbuf(1024);

    auto recv_bytes = ::recv(m_socket, recvbuf.data(), recvbuf.size(), 0);

    m_error = (recv_bytes < 0);

    const std::string empty_str;

    if (m_error)
    {
        return empty_str;
    }

    if (recv_bytes == 0)
    {
        return empty_str;
    }

    std::string::size_type total_chars = recv_bytes;

    std::string str { recvbuf.data(), total_chars };

    return str;
}
