#pragma once

#include "ResponsePacket.h"
#include "../db/DBAdapter.h"
#include "../../plant/Assembly.h"
#include "../../plant/StockPlant.h"

#include <map>

using namespace db;

/// <summary>
/// Process incoming packets, if it is necessary, this class makes a query to database for response some packets.
/// </summary>
class ResponsePacketServerPlant : public ResponsePacket
{
protected:
	DBAdapter* m_lpDBAdapter; /* This object allows to make queries to database. */

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
	ResponsePacketServerPlant(DBAdapter* lpDBAdapter);

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