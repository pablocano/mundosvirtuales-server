#pragma once

#include "ResponsePacket.h"
#include "Loader.h"

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