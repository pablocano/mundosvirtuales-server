#pragma once

#include "PacketComm.h"
#include "Queue.h"
#include "network/TcpComm.h"

#include <mutex>
#include <condition_variable>

class ResponsePacket
{
protected:
	Queue<PacketComm> m_queueResponsePacket;

public:
	ResponsePacket() : m_queueResponsePacket() {};

	ResponsePacket(ResponsePacket const&) = delete;
	ResponsePacket& operator =(ResponsePacket const&) = delete;

	virtual std::unique_ptr<PacketComm> response(PacketComm packet, SocketClientTcp& tcpComm) = 0;

	PacketComm get_response(PacketComm &request)
	{
		std::shared_ptr<PacketComm> response = m_queueResponsePacket.remove(request);
		
		if (response)
		{
			return *response;
		}
		else
		{
			return PacketComm();
		}
	}
};