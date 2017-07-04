#include "ResponsePacketServer.h"
#include "../logger/Logger.h"
#include "../AssemblyUtils.h"

using json = nlohmann::json;

ResponsePacketServerPlant::ResponsePacketServerPlant(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	m_loader(_db_name, _db_user, _db_host, _db_port, _db_password, _db_engine)
{
}

ResponsePacketServerPlant::~ResponsePacketServerPlant()
{
}

std::unique_ptr<PacketComm> ResponsePacketServerPlant::process_packet(PacketComm packet, SocketClientTcp& tcpComm)
{
	std::unique_ptr<PacketComm> responsePacket = nullptr;

	LOGGER_LOG("ResponsePacketServerPlant", tcpComm.getInfo() + ":");
	switch (packet.m_header.m_command)
	{
	case Command::GET_INFO_PLANT:
		LOGGER_LOG("ResponsePacketServerPlant", "Get info plant");
		break;
	case Command::GET_ASSEMBLIES:
		LOGGER_LOG("ResponsePacketServerPlant", "Get machines");
		{
			HeaderPacketComm header;
			PacketComm responsePacket;
			responsePacket.m_header.m_command = Command::RESPONSE_COMMAND;
			responsePacket.m_header.m_idResponse = packet.m_header.m_idResponse;
			responsePacket.m_header.m_statusComm = StatusServer::NORMAL;

			m_mutexLoader.lock();
			Assemblies machines = m_loader.load_machines();
			json j = json{ {"machines", machines} };
			std::string data = j.dump();
			responsePacket.m_lpContent = (char*) data.c_str();
			m_mutexLoader.unlock();

			std::unique_ptr<char[]> packetTCP = responsePacket.packing();
			tcpComm.send(packetTCP.get(), responsePacket.size());
		}
		break;
	case Command::GET_LIST_ASSEMBLIES:
		LOGGER_LOG("ResponsePacketServerPlant", "Get list machines");
		break;
	case Command::GET_MODEL_ASSEMBLIES:
		LOGGER_LOG("ResponsePacketServerPlant", "Get model machines");
		break;
	case Command::GET_INFO_ASSEMBLIES:
		LOGGER_LOG("ResponsePacketServerPlant", "Get info machine");
		break;
	case Command::GET_MAINTENANCE_ASSEMBLY:
		LOGGER_LOG("ResponsePacketServerPlant", "Get maintenance");
		break;
	case Command::GET_LIST_SENSORS:
		LOGGER_LOG("ResponsePacketServerPlant", "Get list sensors");
		break;
	case Command::GET_SENSORS:
		LOGGER_LOG("ResponsePacketServerPlant", "Get sensors");
		break;
	case Command::CLOSE_CONNECTION:
		LOGGER_LOG("ResponsePacketServerPlant", "Close Connection");
		break;
	case Command::RESPONSE_COMMAND:
		LOGGER_LOG("ResponsePacketServerPlant", "RESPONSE");
		m_queueResponsePacket.add(packet);
		break;
	case Command::NONE:
	default:
		LOGGER_LOG("ResponsePacketServerPlant", "None command");
	}

	if (packet.sizeContent() > 0)
		LOGGER_LOG("ResponsePacketServerPlant", packet.m_lpContent);

	return responsePacket;
}