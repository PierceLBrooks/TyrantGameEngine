/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Network/Win32/SocketImpl.hpp>
#include <cstring>


namespace TGE
{
namespace priv
{
////////////////////////////////////////////////////////////
sockaddr_in SocketImpl::createAddress(Uint32 address, unsigned short port)
{
    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = htonl(address);
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);

    return addr;
}


////////////////////////////////////////////////////////////
SocketHandle SocketImpl::invalidSocket()
{
    return INVALID_SOCKET;
}


////////////////////////////////////////////////////////////
void SocketImpl::close(SocketHandle sock)
{
    closesocket(sock);
}


////////////////////////////////////////////////////////////
void SocketImpl::setBlocking(SocketHandle sock, bool block)
{
    u_long blocking = block ? 0 : 1;
    ioctlsocket(sock, FIONBIO, &blocking);
}


////////////////////////////////////////////////////////////
Socket::Status SocketImpl::getErrorStatus()
{
    switch (WSAGetLastError())
    {
        case WSAEWOULDBLOCK :  return Socket::NotReady;
        case WSAEALREADY :     return Socket::NotReady;
        case WSAECONNABORTED : return Socket::Disconnected;
        case WSAECONNRESET :   return Socket::Disconnected;
        case WSAETIMEDOUT :    return Socket::Disconnected;
        case WSAENETRESET :    return Socket::Disconnected;
        case WSAENOTCONN :     return Socket::Disconnected;
        case WSAEISCONN :      return Socket::Done; // when connecting a non-blocking socket
        default :              return Socket::Error;
    }
}


////////////////////////////////////////////////////////////
// Windows needs some initialization and cleanup to get
// sockets working properly... so let's create a class that will
// do it automatically
////////////////////////////////////////////////////////////
struct SocketInitializer
{
    SocketInitializer()
    {
        WSADATA init;
        WSAStartup(MAKEWORD(2, 2), &init);
    }

    ~SocketInitializer()
    {
        WSACleanup();
    }
};

SocketInitializer globalInitializer;

} // namespace priv

} // namespace TGE
