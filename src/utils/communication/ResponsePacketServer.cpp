#include "ResponsePacketServer.h"
#include "../logger/Logger.h"
#include "../AssemblyUtils.h"
#include "../../plant/StockPlant.h"

using json = nlohmann::json;

ResponsePacketServerPlant::ResponsePacketServerPlant(DBAdapter* lpDBAdapter) : m_lpDBAdapter(lpDBAdapter)
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
			Assemblies::getInstance().updateDictAssembliesFromDB(m_lpDBAdapter);
			StockPlant plant = StockPlant::loadStockPlant(m_lpDBAdapter);
			
			json j = json{ {"plant", plant} };
			std::string data = j.dump();

			sendResponse(tcpComm, packet, (char*)data.c_str());
		}
		break;
	case Command::GET_PLANT:
		LOGGER_LOG("ResponsePacketServerPlant", "Get PLANT");
		{
			Assemblies::getInstance().updateDictAssembliesFromDB(m_lpDBAdapter);
			StockPlant plant = StockPlant::loadStockPlant(m_lpDBAdapter);

			json j = json{ { "plant", plant } };
			std::string data = j.dump();

			sendResponse(tcpComm, packet, (char*)data.c_str());
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
	case Command::GET_VERSION_ID:
		LOGGER_LOG("ResponsePacketServerPlant", "Get Version ID");
		{
			try
			{
				json parseJSON = json::parse(packet.m_lpContent);
				int version = parseJSON.at("version").get<int>();
				int assembly_id = parseJSON.at("id").get<int>();
				
				json j = json{ { "version", version + 1 }, { "id", assembly_id } };
				std::string data = j.dump();
					
				sendResponse(tcpComm, packet, (char *) data.c_str(), StatusServer::OK_RESPONSE);
			}
			catch (const std::exception &e)
			{
				LOGGER_ERROR("ResponsePacketServerPlant", e.what());
				sendResponse(tcpComm, packet, nullptr, StatusServer::ERROR_RESPONSE);
			}
		}
		break;
	case Command::NEW_ASSEMBLY:
		LOGGER_LOG("ResponsePacketServerPlant", "New Assembly");
		{
			try
			{
				json parseJSON = json::parse(packet.m_lpContent);
				std::string s(packet.m_lpContent);
				AssemblyComm assembly = parseJSON;
				
				// TODO: Save assembly.
				
				json j = json{ { "id", 3 } };
				std::string data = j.dump();

				sendResponse(tcpComm, packet, (char *)data.c_str(), StatusServer::OK_RESPONSE);
			}
			catch (const std::exception &e)
			{
				LOGGER_ERROR("ResponsePacketServerPlant", e.what());
				sendResponse(tcpComm, packet, nullptr, StatusServer::ERROR_RESPONSE);
			}
		}
		break;
	case Command::UPDATE_ASSEMBLY:
		LOGGER_LOG("ResponsePacketServerPlant", "Update Assembly");
		{
			try
			{
				/*json parseJSON = json::parse(packet.m_lpContent);
				int version = parseJSON.at("version").get<int>();
				int assembly_id = parseJSON.at("id").get<int>();

				json j = json{ { "version", version + 1 },{ "id", assembly_id } };
				std::string data = j.dump();*/

				sendResponse(tcpComm, packet, nullptr, StatusServer::OK_RESPONSE);
			}
			catch (const std::exception &e)
			{
				LOGGER_ERROR("ResponsePacketServerPlant", e.what());
				sendResponse(tcpComm, packet, nullptr, StatusServer::ERROR_RESPONSE);
			}
		}
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