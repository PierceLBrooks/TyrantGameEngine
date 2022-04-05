/*************************************/
/** Copyright © 2014 Coldsnap Games **/
/*************************************/

#ifndef TGE_TCPSOCKET_HPP
#define TGE_TCPSOCKET_HPP

/*************************************/
/**             Headers             **/
/*************************************/
#include <Tyrant/Config.hpp>
#include <Tyrant/Network/Socket.hpp>
#include <Tyrant/System/Time.hpp>


namespace TGE
{
class TcpListener;
class IpAddress;
class Packet;

////////////////////////////////////////////////////////////
/// \brief Specialized socket using the TCP protocol
///
////////////////////////////////////////////////////////////
class TGE_API TcpSocket : public Socket
{
public :

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    ////////////////////////////////////////////////////////////
    TcpSocket();

    ////////////////////////////////////////////////////////////
    /// \brief Get the port to which the socket is bound locally
    ///
    /// If the socket is not connected, this function returns 0.
    ///
    /// \return Port to which the socket is bound
    ///
    /// \see connect, getRemotePort
    ///
    ////////////////////////////////////////////////////////////
    unsigned short getLocalPort() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the address of the connected peer
    ///
    /// It the socket is not connected, this function returns
    /// TGE::IpAddress::None.
    ///
    /// \return Address of the remote peer
    ///
    /// \see getRemotePort
    ///
    ////////////////////////////////////////////////////////////
    IpAddress getRemoteAddress() const;

    ////////////////////////////////////////////////////////////
    /// \brief Get the port of the connected peer to which
    ///        the socket is connected
    ///
    /// If the socket is not connected, this function returns 0.
    ///
    /// \return Remote port to which the socket is connected
    ///
    /// \see getRemoteAddress
    ///
    ////////////////////////////////////////////////////////////
    unsigned short getRemotePort() const;

    ////////////////////////////////////////////////////////////
    /// \brief Connect the socket to a remote peer
    ///
    /// In blocking mode, this function may take a while, especially
    /// if the remote peer is not reachable. The last parameter allows
    /// you to stop trying to connect after a given timeout.
    /// If the socket was previously connected, it is first disconnected.
    ///
    /// \param remoteAddress Address of the remote peer
    /// \param remotePort    Port of the remote peer
    /// \param timeout       Optional maximum time to wait
    ///
    /// \return Status code
    ///
    /// \see disconnect
    ///
    ////////////////////////////////////////////////////////////
    Status connect(const IpAddress& remoteAddress, unsigned short remotePort, Time timeout = Time::Zero);

    ////////////////////////////////////////////////////////////
    /// \brief Disconnect the socket from its remote peer
    ///
    /// This function gracefully closes the connection. If the
    /// socket is not connected, this function has no effect.
    ///
    /// \see connect
    ///
    ////////////////////////////////////////////////////////////
    void disconnect();

    ////////////////////////////////////////////////////////////
    /// \brief Send raw data to the remote peer
    ///
    /// This function will fail if the socket is not connected.
    ///
    /// \param data Pointer to the sequence of bytes to send
    /// \param size Number of bytes to send
    ///
    /// \return Status code
    ///
    /// \see receive
    ///
    ////////////////////////////////////////////////////////////
    Status send(const void* data, std::size_t size);

    ////////////////////////////////////////////////////////////
    /// \brief Receive raw data from the remote peer
    ///
    /// In blocking mode, this function will wait until some
    /// bytes are actually received.
    /// This function will fail if the socket is not connected.
    ///
    /// \param data     Pointer to the array to fill with the received bytes
    /// \param size     Maximum number of bytes that can be received
    /// \param received This variable is filled with the actual number of bytes received
    ///
    /// \return Status code
    ///
    /// \see send
    ///
    ////////////////////////////////////////////////////////////
    Status receive(void* data, std::size_t size, std::size_t& received);

    ////////////////////////////////////////////////////////////
    /// \brief Send a formatted packet of data to the remote peer
    ///
    /// This function will fail if the socket is not connected.
    ///
    /// \param packet Packet to send
    ///
    /// \return Status code
    ///
    /// \see receive
    ///
    ////////////////////////////////////////////////////////////
    Status send(Packet& packet);

    ////////////////////////////////////////////////////////////
    /// \brief Receive a formatted packet of data from the remote peer
    ///
    /// In blocking mode, this function will wait until the whole packet
    /// has been received.
    /// This function will fail if the socket is not connected.
    ///
    /// \param packet Packet to fill with the received data
    ///
    /// \return Status code
    ///
    /// \see send
    ///
    ////////////////////////////////////////////////////////////
    Status receive(Packet& packet);

private:

    friend class TcpListener;

    ////////////////////////////////////////////////////////////
    /// \brief Structure holding the data of a pending packet
    ///
    ////////////////////////////////////////////////////////////
    struct PendingPacket
    {
        PendingPacket();

        Uint32            Size;         ///< Data of packet size
        std::size_t       SizeReceived; ///< Number of size bytes received so far
        std::vector<char> Data;         ///< Data of the packet
    };

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    PendingPacket m_pendingPacket; ///< Temporary data of the packet currently being received
};

} // namespace TGE


#endif // TGE_TCPSOCKET_HPP


////////////////////////////////////////////////////////////
/// \class TGE::TcpSocket
/// \ingroup network
///
/// TCP is a connected protocol, which means that a TCP
/// socket can only communicate with the host it is connected
/// to. It can't send or receive anything if it is not connected.
///
/// The TCP protocol is reliable but adds a slight overhead.
/// It ensures that your data will always be received in order
/// and without errors (no data corrupted, lost or duplicated).
///
/// When a socket is connected to a remote host, you can
/// retrieve informations about this host with the
/// getRemoteAddress and getRemotePort functions. You can
/// also get the local port to which the socket is bound
/// (which is automatically chosen when the socket is connected),
/// with the getLocalPort function.
///
/// Sending and receiving data can use either the low-level
/// or the high-level functions. The low-level functions
/// process a raw sequence of bytes, and cannot ensure that
/// one call to Send will exactly match one call to Receive
/// at the other end of the socket.
///
/// The high-level interface uses packets (see TGE::Packet),
/// which are easier to use and provide more safety regarding
/// the data that is exchanged. You can look at the TGE::Packet
/// class to get more details about how they work.
///
/// The socket is automatically disconnected when it is destroyed,
/// but if you want to explicitely close the connection while
/// the socket instance is still alive, you can call disconnect.
///
/// Usage example:
/// \code
/// // ----- The client -----
///
/// // Create a socket and connect it to 192.168.1.50 on port 55001
/// TGE::TcpSocket socket;
/// socket.connect("192.168.1.50", 55001);
///
/// // Send a message to the connected host
/// std::string message = "Hi, I am a client";
/// socket.send(message.c_str(), message.size() + 1);
///
/// // Receive an answer from the server
/// char buffer[1024];
/// std::size_t received = 0;
/// socket.receive(buffer, sizeof(buffer), received);
/// std::cout << "The server said: " << buffer << std::endl;
///
/// // ----- The server -----
///
/// // Create a listener to wait for incoming connections on port 55001
/// TGE::TcpListener listener;
/// listener.listen(55001);
///
/// // Wait for a connection
/// TGE::TcpSocket socket;
/// listener.accept(socket);
/// std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
///
/// // Receive a message from the client
/// char buffer[1024];
/// std::size_t received = 0;
/// socket.receive(buffer, sizeof(buffer), received);
/// std::cout << "The client said: " << buffer << std::endl;
///
/// // Send an answer
/// std::string message = "Welcome, client";
/// socket.send(message.c_str(), message.size() + 1);
/// \endcode
///
/// \see TGE::Socket, TGE::UdpSocket, TGE::Packet
///
////////////////////////////////////////////////////////////
