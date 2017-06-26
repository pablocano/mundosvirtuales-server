#include "ResponsePacketClient.h"
#include "../serialization/json.h"
#include "../logger/Logger.h"

#include <iostream>

using json = nlohmann::json;

ResponsePacketClientPlant::ResponsePacketClientPlant()
{
	LOGGER_LOG("Response Packet Client", "Create client");
}

ResponsePacketClientPlant::~ResponsePacketClientPlant()
{
	LOGGER_LOG("Response Packet Client", "Delete obj client");
}

std::unique_ptr<PacketComm> ResponsePacketClientPlant::process_packet(PacketComm packet, SocketClientTcp& tcpComm)
{
	std::unique_ptr<PacketComm> responsePacket = nullptr;
	
	LOGGER_LOG("Response Packet Client", tcpComm.getInfo() + ":");
	switch (packet.m_header.m_command)
	{
	case Command::CLOSE_CONNECTION:
		LOGGER_LOG("Response Packet Client", "Close Connection");
		tcpComm.closeSocket();
		break;
	case Command::RESPONSE_COMMAND:
		LOGGER_LOG("Response Packet Client", "RESPONSE");
		m_queueResponsePacket.add(packet);
		break;
	case Command::NONE:
	default:
		LOGGER_LOG("Response Packet Client", "None command");
	}

	return responsePacket;
}
