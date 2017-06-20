#pragma once

#include "ResponsePacket.h"
#include "Loader.h"

/// <summary>
/// Process incoming packets, if it is necessary, this class makes a query to database for response some packets.   
/// </summary>
class ResponsePacketServerPlant : public ResponsePacket
{
protected:
	Loader m_loader; /* This object allows to make queries to database. */
	std::mutex m_mutexLoader; /* Mutex for uses object loader. */
	std::condition_variable m_condvLoader; /* Condition for uses object loader. */

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
	ResponsePacketServerPlant(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");
	
	/// <summary>
	/// Destructor.
	/// </summary>
	~ResponsePacketServerPlant();

	/// <summary>
	/// Automatically respond incoming packets, if a packet respond to a request packet, the packet is placed in a packet queue.
	/// </summary>
	/// <param name="packet">Incoming packet.</param>
	/// <param name="tcpComm">Socket communication.</param>
	/// <returns>Returns response packet for the incoming packet.</returns>
	std::unique_ptr<PacketComm> process_packet(PacketComm packet, SocketClientTcp& tcpComm);
};