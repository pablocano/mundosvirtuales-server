#pragma once

#include "ResponsePacket.h"

class ResponsePacketClientPlant : public ResponsePacket
{
protected:
	std::mutex m_mutex;
	std::condition_variable m_condv;

public:
	ResponsePacketClientPlant();
	~ResponsePacketClientPlant();

	std::unique_ptr<PacketComm> response(PacketComm packet, SocketClientTcp& tcpComm);
};
