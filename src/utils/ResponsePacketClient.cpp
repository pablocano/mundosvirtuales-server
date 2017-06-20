#include "ResponsePacketClient.h"
#include "serialization/json.h"

#include <iostream>

using json = nlohmann::json;

ResponsePacketClientPlant::ResponsePacketClientPlant()
{
	std::cout << "Create client" << std::endl;
}

ResponsePacketClientPlant::~ResponsePacketClientPlant()
{
	std::cout << "Delete obj client" << std::endl;
}

std::unique_ptr<PacketComm> ResponsePacketClientPlant::process_packet(PacketComm packet, SocketClientTcp& tcpComm)
{
	std::unique_ptr<PacketComm> responsePacket = nullptr;
	
	std::cout << tcpComm.getInfo() << ":" << std::endl;
	switch (packet.m_header.m_command)
	{
	case Command::CLOSE_CONNECTION:
		std::cout << "Close Connection" << std::endl;
		tcpComm.closeSocket();
		break;
	case Command::RESPONSE_COMMAND:
		std::cout << "RESPONSE" << std::endl;
		m_queueResponsePacket.add(packet);
		break;
	case Command::NONE:
	default:
		std::cout << "None command" << std::endl;
	}

	return responsePacket;
}
