#pragma once

#ifdef WIN32

#ifdef NOMINMAX
#include <winsock.h>
#else
#define NOMINMAX
#include <winsock.h>
#undef NOMINMAX
#endif

#else
#include <netinet/in.h>
#endif

#ifdef WIN32
	#define CLOSE(socket) closesocket(socket)
#else
	#define CLOSE(socket) close(socket)
#endif

class SocketClientTcp;

class SocketTCP
{
protected:
	sockaddr_in m_address; /**< The socket m_address. */
	int m_socket; /**< The handle of the basic socket. */

public:
	/**
	* Constructor
	*/
	SocketTCP() : m_socket(0) {}

	/**
	* The function closes the transfer socket.
	*/
	void closeSocket()
	{
		if (m_socket > 0)
			CLOSE(m_socket);
		m_socket = 0;
	}

	char* getInfo()
	{
		return inet_ntoa(m_address.sin_addr);
	}

	int getSocket() { return m_socket; }
};

class SocketServerTCP : public SocketTCP
{
public:
	/**
	* Constructor.
	* Opens a TCP connection to a remote host.
	* @param port The port over which will be communicated.
	*/
	SocketServerTCP(int port);

	/**
	* Destructor.
	*/
	~SocketServerTCP();

	/**
	* Accept Clients
	* @return Socket client
	*/
	int acceptClient();

	/**
	* The function return whether the connection was successful.
	* @return Was the connection established?
	*/
	bool connected() const { return m_socket > 0; }

protected:
	/**
	* Binds the socket to an m_address and port number a server call
	* @return binding on m_address was successful?
	*/
	bool bindSocket();

	/**
	* Listens to connecting clients, a server call
	* @return listening client was successful?
	*/
	bool listenSocket();
};


/**
 * @class SocketClientTcp
 * The class implements a tcp connection.
 */
class SocketClientTcp : public SocketTCP
{
private:
	bool m_bWasConnected; /**< Whether a tranfer connection was established or not */

	/**
	* The method checks whether the connection is available.
	* If not, it tries to reestablish it.
	* @return Can the connection be used now?
	*/
	bool checkConnection();

public:
	/**
	 * Constructor.
	 * Opens a TCP connection to a remote host.
	 * @param ip The ip m_address of the communication partner. If 0, the port
	 *           will be opened as server.
	 * @param port The port over which will be communicated.
	 */
	SocketClientTcp(const char* ip, int port);

	/**
	* Constructor.
	* Opens a TCP connection to a remote host.
	* @param socket.
	*/
	SocketClientTcp(int socketClient);

	/**
	 * Destructor.
	 */
	~SocketClientTcp();

	/**
	* The function sends a block of bytes.
	* It will return immediately unless the send buffer is full.
	* @param buffer The bytes to send.
	* @param size The number of bytes to send.
	* @return Was the data successfully sent?
	*/
	bool send(const char* buffer, int size);

	/**
	* The function receives a block of bytes.
	* @param buffer This buffer will be filled with the bytes to receive.
	*               It must provide at least "size" bytes.
	* @param size The number of bytes to receive.
	* @param wait The method will wait until all bytes have been received.
	*             However, it can still fail if the connection is broken.
	* Was the data successfully received?
	*/
	bool receive(char* buffer, int size, bool wait = true);

	/**
	* The function receives a block of bytes using the system call recv().
	* @param buffer This buffer will be filled with the bytes to receive.
	* @param size of buffer.
	* @param flags See RECV(2) manpage.
	* @return number of bytes received, or -1 on error.
	*/
	int receiveSys(void* buffer, unsigned int size, int flags);

	/**
	* The function return whether the connection was successful.
	* @return Was the connection established?
	*/
	bool connected() const { return m_socket > 0; }
};
