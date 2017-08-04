#include "ClientPlant.h"
#include "utils/platform/SystemCall.h"
#include "utils/logger/Logger.h"

#include <exception>

using json = nlohmann::json;

bool ClientPlant::requestPlant()
{
	try
	{
		if (requestAssemblies())
		{
			std::string response = request(Command::GET_PLANT);
			json parseJSON = json::parse(response.c_str());
			Plant::getInstance().setPlant(parseJSON.at("plant"));

			return !response.empty();
		}
	}
	catch (std::exception e)
	{
		LOGGER_ERROR("Response Packet Client", e.what());
	}

	return false;
}

bool ClientPlant::requestAssemblies()
{
	if (!isLoadedAssemblies())
	{
		std::string response = request(Command::GET_ASSEMBLIES);
		json parseJSON = json::parse(response.c_str());
		Assemblies::getInstance().setAssemblies(parseJSON.at("assemblies"));
		
		m_enableAssembly = !response.empty();
	}

	return m_enableAssembly;
}

bool ClientPlant::isLoadedAssemblies() const
{
	return m_enableAssembly;
}

std::string ClientPlant::request(Command command, std::string s, int timeout)
{
	if (m_tcpComm.connected())
	{
		HeaderPacketComm header;
		PacketComm packetRequest;
		packetRequest.m_header.m_command = command;
		packetRequest.m_lpContent = (char *)s.c_str();

		std::unique_ptr<char[]> packetTCP = packetRequest.packing();
		m_tcpComm.send(packetTCP.get(), packetRequest.size());

		SystemCall::sleep(timeout);

		PacketComm packetResponse = m_lpResponsePacket->get_response(packetRequest);

		if ((packetResponse.m_header.m_idResponse == packetRequest.m_header.m_idResponse) && packetResponse.sizeContent() > 0)
		{
			return packetResponse.m_lpContent;
		}
	}

	return ""; // Empty
}