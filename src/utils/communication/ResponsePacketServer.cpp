#include "ResponsePacketServer.h"
#include "../logger/Logger.h"
#include "../AssemblyUtils.h"

using json = nlohmann::json;

ResponsePacketServerPlant::ResponsePacketServerPlant(DBAdapter* lpDBAdapter) : m_lpDBAdapter(lpDBAdapter)
{
	Assemblies::getInstance().loadDictAssembliesFromDB(m_lpDBAdapter);
	Plant::getInstance().loadPlantFromDB(m_lpDBAdapter);
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
		LOGGER_LOG("ResponsePacketServerPlant", "GET_INFO_PLANT");
		break;
	case Command::GET_ASSEMBLIES: // OK
		LOGGER_LOG("ResponsePacketServerPlant", "GET_ASSEMBLIES");
		{
			Assemblies::getInstance().updateDictAssembliesFromDB(m_lpDBAdapter);
			
			json j = json{ {"assemblies", Assemblies::getInstance() } };
			std::string data = j.dump();

			sendResponse(tcpComm, packet, (char*)data.c_str());
		}
		break;
	case Command::GET_PLANT: // OK
		LOGGER_LOG("ResponsePacketServerPlant", "GET_PLANT");
		{
			json j = json{ { "plant", Plant::getInstance() } };
			std::string data = j.dump();

			sendResponse(tcpComm, packet, (char*)data.c_str());
		}
		break;
	case Command::GET_MODEL_ASSEMBLY:
		LOGGER_LOG("ResponsePacketServerPlant", "GET_MODEL_ASSEMBLY");
		break;
	case Command::GET_INFO_ASSEMBLY:
		LOGGER_LOG("ResponsePacketServerPlant", "GET_INFO_ASSEMBLY");
		break;
	case Command::GET_MAINTENANCE_ASSEMBLY:
		LOGGER_LOG("ResponsePacketServerPlant", "GET_MAINTENANCE_ASSEMBLY");
		break;
	case Command::GET_LIST_SENSORS:
		LOGGER_LOG("ResponsePacketServerPlant", "GET_LIST_SENSORS");
		break;
	case Command::GET_SENSORS:
		LOGGER_LOG("ResponsePacketServerPlant", "GET_SENSORS");
		break;
	case Command::CLOSE_CONNECTION: // OK
		LOGGER_LOG("ResponsePacketServerPlant", "CLOSE_CONNECTION");
		tcpComm.closeSocket();
		break;
	case Command::GET_VERSION_ID: // OK
		LOGGER_LOG("ResponsePacketServerPlant", "GET_VERSION_ID");
		{
			try
			{
				json parseJSON = json::parse(packet.m_lpContent);
				int version = parseJSON.at("version").get<int>();
				int assembly_id = parseJSON.at("id").get<int>();
				
				std::string data;
				if (Assemblies::getInstance().existAssembly(assembly_id))
				{
					const Assembly& assembly = Assemblies::getInstance()[assembly_id];
					json j = json{ { "version", assembly.getModel().getVersion() },{ "id", assembly_id } };
					data = j.dump();
				}
				else
				{
					json j = json{ { "version", -1 },{ "id", assembly_id } };
					data = j.dump();
				}
					
				sendResponse(tcpComm, packet, (char *) data.c_str(), StatusServer::OK_RESPONSE);
			}
			catch (const std::exception &e)
			{
				LOGGER_ERROR("ResponsePacketServerPlant", e.what());
				sendResponse(tcpComm, packet, nullptr, StatusServer::ERROR_RESPONSE);
			}
		}
		break;
	case Command::NEW_ASSEMBLY: // OK
		LOGGER_LOG("ResponsePacketServerPlant", "NEW_ASSEMBLY");
		{
			try
			{
				json parseJSON = json::parse(packet.m_lpContent);
				AssemblyComm assemblyComm = parseJSON;
				
				int idAssembly = assemblyComm.m_id_assembly;

				if (!Assemblies::getInstance().existAssembly(idAssembly))
				{
					idAssembly = Assemblies::getInstance().createAssembly(m_lpDBAdapter, assemblyComm); // Update ID Assembly
					assemblyComm.m_id_assembly = idAssembly;					
				}
				
				if(!assemblyComm.isOnlyAssembly() && idAssembly > 0)
				{
					Assemblies::getInstance().processRelation(m_lpDBAdapter, assemblyComm);
				}

				if (idAssembly > 0)
				{
					Plant::getInstance().UpdateTree(m_lpDBAdapter, assemblyComm);
				}
				
				json j = json{ { "id", idAssembly } };
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
		LOGGER_LOG("ResponsePacketServerPlant", "UPDATE_ASSEMBLY");
		{
			try
			{
				json parseJSON = json::parse(packet.m_lpContent);
				AssemblyComm assemblyComm = parseJSON;

				int idAssembly = assemblyComm.m_id_assembly;

				if (Assemblies::getInstance().existAssembly(idAssembly))
				{
					Assemblies::getInstance().updateAssembly(m_lpDBAdapter, assemblyComm);
					Plant::getInstance().UpdateTree(m_lpDBAdapter, assemblyComm);
				}
				else
				{
					idAssembly = -1;
				}

				json j = json{ { "id", idAssembly } };
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
	case Command::NONE:
	default:
		LOGGER_LOG("ResponsePacketServerPlant", "NONE");
	}

	return responsePacket;
}