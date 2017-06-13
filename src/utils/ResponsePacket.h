#pragma once

#include "PacketComm.h"
#include "Loader.h"
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

class ResponsePacketServerPlant : public ResponsePacket
{
protected:
	Loader m_loader;
	std::mutex m_mutexLoader;
	std::condition_variable m_condvLoader;

public:
	ResponsePacketServerPlant(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");
	~ResponsePacketServerPlant();

	std::unique_ptr<PacketComm> response(PacketComm packet, SocketClientTcp& tcpComm);
};