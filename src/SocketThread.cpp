#include <Windows.h>

#include "SocketThread.h"
#include "SocketThread_priv.h"


SocketThread::SocketThread(const struct ThreadParams &params) :
    m_priv { new SocketThread_priv(params) }
{
}

SocketThread::~SocketThread()
{
    delete m_priv;
}

void SocketThread::start()
{
    m_priv->start();
}
