/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Network/TcpListener.hpp>
#include <Tyrant/Network/TcpSocket.hpp>
#include <Tyrant/Network/SocketImpl.hpp>
#include <Tyrant/System/Log.hpp>


namespace TGE
{
////////////////////////////////////////////////////////////
TcpListener::TcpListener() :
Socket(Tcp)
{

}


////////////////////////////////////////////////////////////
unsigned short TcpListener::getLocalPort() const
{
    if (getHandle() != priv::SocketImpl::invalidSocket())
    {
        // Retrieve informations about the local end of the socket
        sockaddr_in address;
        priv::SocketImpl::AddrLength size = sizeof(address);
        if (getsockname(getHandle(), reinterpret_cast<sockaddr*>(&address), &size) != -1)
        {
            return ntohs(address.sin_port);
        }
    }

    // We failed to retrieve the port
    return 0;
}


////////////////////////////////////////////////////////////
Socket::Status TcpListener::listen(unsigned short port)
{
    // Create the internal socket if it doesn't exist
    create();

    // Bind the socket to the specified port
    sockaddr_in address = priv::SocketImpl::createAddress(INADDR_ANY, port);
    if (bind(getHandle(), reinterpret_cast<sockaddr*>(&address), sizeof(address)) == -1)
    {
        // Not likely to happen, but...
        Log() << "Failed to bind listener socket to port " << port << std::endl;
        return Error;
    }

    // Listen to the bound port
    if (::listen(getHandle(), 0) == -1)
    {
        // Oops, socket is deaf
        Log() << "Failed to listen to port " << port << std::endl;
        return Error;
    }

    return Done;
}


////////////////////////////////////////////////////////////
void TcpListener::close()
{
    // Simply close the socket
    Socket::close();
}


////////////////////////////////////////////////////////////
Socket::Status TcpListener::accept(TcpSocket& socket)
{
    // Make sure that we're listening
    if (getHandle() == priv::SocketImpl::invalidSocket())
    {
        Log() << "Failed to accept a new connection, the socket is not listening" << std::endl;
        return Error;
    }

    // Accept a new connection
    sockaddr_in address;
    priv::SocketImpl::AddrLength length = sizeof(address);
    SocketHandle remote = ::accept(getHandle(), reinterpret_cast<sockaddr*>(&address), &length);

    // Check for errors
    if (remote == priv::SocketImpl::invalidSocket())
        return priv::SocketImpl::getErrorStatus();

    // Initialize the new connected socket
    socket.close();
    socket.create(remote);

    return Done;
}

} // namespace TGE
