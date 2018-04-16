#include <ws2tcpip.h>
#include <string>

#include "AddrInfo.h"
#include "SocketListener.h"


SocketListener::SocketListener(const AddrInfo &ai) :
    m_error { false },
    m_socket { socket(ai.get()->ai_family, ai.get()->ai_socktype, ai.get()->ai_protocol) }
{
    m_error = (m_socket == INVALID_SOCKET);

    if (!m_error)
    {
        auto result = bind(m_socket, ai.get()->ai_addr, (int)ai.get()->ai_addrlen);

        m_error = (result == SOCKET_ERROR);
    }
}

SocketListener::~SocketListener()
{
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
    }
}

bool SocketListener::is_error() const
{
    return m_error;
}

void SocketListener::listen()
{
    auto result = ::listen(m_socket, SOMAXCONN);

    m_error = (result == SOCKET_ERROR);
}

void SocketListener::close()
{
    if (m_socket != INVALID_SOCKET)
    {
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}

SOCKET SocketListener::accept()
{
    return ::accept(m_socket, NULL, NULL);
}
