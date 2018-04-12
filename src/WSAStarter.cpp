#include <winsock2.h>

#include "WSAStarter.h"


WSAStarter::WSAStarter() :
    m_data {},
    m_error { false }
{
    auto result = WSAStartup(MAKEWORD(2,2), &m_data);

    m_error = (result != 0);
}

WSAStarter::~WSAStarter()
{
    WSACleanup();
}

bool WSAStarter::is_error() const
{
    return m_error;
}
