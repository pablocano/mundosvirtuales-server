#pragma once

#include "TcpComm.h"
#include "ClientTCP.h"
#include "../Runnable.h"
#include "../Queue.h"
#include "../ResponsePacketServer.h"

/// <summary>
/// Server TCP handles tcp communication for each remote client.
/// </summary>
class ServerTCP : public Runnable
{
protected:
	SocketServerTCP* m_lpTcpServer; /* Socket server. */
	std::shared_ptr<ResponsePacketServerPlant> m_lpServerPlant; /* Pointer to class responding to messages (packet).  */
	Queue<std::shared_ptr<ClientTCP>> m_queueClients; /* Queue of active clients (where each client is attended with a thread). */

	void run(); /* Implement virtual function. */

	/// <summary>
	/// Remove client from queue.
	/// </summary>
	/// <param name="lpClient">Pointer to handle communication with client.</param>
	void remove_client_from_queue(std::shared_ptr<ClientTCP> lpClient);

public:
	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="_db_name">Name database.</param>
	/// <param name="_db_user">User database.</param>
	/// <param name="_db_host">Remote host where is databse.</param>
	/// <param name="_db_port">Port remote host.</param>
	/// <param name="_db_password">Password user database.</param>
	/// <param name="_db_engine">Engine database (default: PostgreSQL).</param>
	ServerTCP(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");
	
	/// <summary>
	/// Destructor.
	/// </summary>
	~ServerTCP();

	/// <summary>
	/// Determine if exist clients.
	/// </summary>
	/// <returns>Returns true if there are active clients, false otherwise.</returns>
	bool exist_client();

	/// <summary>
	/// Close all active clients.
	/// </summary>
	void close_all_clients();
};
