#pragma once

#include "ResponsePacket.h"

/// <summary>
/// Process incoming packets for client.
/// </summary>
class ResponsePacketClientPlant : public ResponsePacket
{
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	ResponsePacketClientPlant();

	/// <summary>
	/// Destructor.
	/// </summary>
	~ResponsePacketClientPlant();

	/// <summary>
	/// Automatically respond incoming packets, if a packet respond to a request packet, the packet is placed in a packet queue.
	/// </summary>
	/// <param name="packet">Incoming packet.</param>
	/// <param name="tcpComm">Socket communication.</param>
	/// <returns>Returns response packet for the incoming packet.</returns>
	std::unique_ptr<PacketComm> process_packet(PacketComm packet, SocketClientTcp& tcpComm);
};
