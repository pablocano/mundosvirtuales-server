#include "ResponsePacketServer.h"

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

	std::cout << tcpComm.getInfo() << ":" << std::endl;
	switch (packet.m_header.m_command)
	{
	case Command::GET_INFO_PLANT:
		std::cout << "Get info plant" << std::endl;
		break;
	case Command::GET_MACHINES:
		std::cout << "Get machines" << std::endl;
		{
			HeaderPacketComm header;
			PacketComm responsePacket;
			responsePacket.m_header.m_command = Command::RESPONSE_COMMAND;
			responsePacket.m_header.m_idResponse = packet.m_header.m_idResponse;
			responsePacket.m_header.m_statusComm = StatusServer::NORMAL;

			m_mutexLoader.lock();
			Machines machines = m_loader.load_machines();
			json j = json{ {"machines", machines} };
			std::string data = j.dump();
			responsePacket.m_lpContent = (char*) data.c_str();
			m_mutexLoader.unlock();

			//std::cout << responsePacket.m_lpContent << std::endl;

			std::unique_ptr<char[]> packetTCP = responsePacket.packing();
			tcpComm.send(packetTCP.get(), responsePacket.size());
		}
		break;
	case Command::GET_LIST_MACHINES:
		std::cout << "Get list machines" << std::endl;
		break;
	case Command::GET_MODEL_MACHINES:
		std::cout << "Get model machines" << std::endl;
		break;
	case Command::GET_INFO_MACHINE:
		std::cout << "Get info machine" << std::endl;
		break;
	case Command::GET_MAINTENANCE_MACHINE:
		std::cout << "Get maintenance" << std::endl;
		break;
	case Command::GET_LIST_SENSORS:
		std::cout << "Get list sensors" << std::endl;
		break;
	case Command::GET_SENSORS:
		std::cout << "Get sensors" << std::endl;
		break;
	case Command::CLOSE_CONNECTION:
		std::cout << "Close Connection" << std::endl;
		break;
	case Command::RESPONSE_COMMAND:
		std::cout << "RESPONSE" << std::endl;
		m_queueResponsePacket.add(packet);
		break;
	case Command::NONE:
	default:
		std::cout << "None command" << std::endl;
	}

	if (packet.sizeContent() > 0)
		std::cout << packet.m_lpContent << std::endl;

	return responsePacket;
}