#include "ClientPlant.h"
#include "utils/platform/SystemCall.h"
#include "utils/logger/Logger.h"

#include <exception>

using json = nlohmann::json;

bool ClientPlant::requestPlant(Plant& plant)
{

	if (m_tcpComm.connected())
	{
		HeaderPacketComm header;
		PacketComm packetRequest;
		packetRequest.m_header.m_command = Command::GET_PLANT;

		std::unique_ptr<char[]> packetTCP = packetRequest.packing();
		m_tcpComm.send(packetTCP.get(), packetRequest.size());

		PacketComm packetResponse = m_lpResponsePacket->get_response(packetRequest);

		if ((packetResponse.m_header.m_idResponse == packetRequest.m_header.m_idResponse) && packetResponse.sizeContent() > 0)
		{
			try
			{
				json parseJSON = json::parse(packetResponse.m_lpContent);
				plant.setPlant(parseJSON.at("plant"));
			}
			catch (std::exception e)
			{
				LOGGER_ERROR("Response Packet Client", e.what());
			}
		}
			
	}	

	return plant.getPlant().isEnable();
}

bool ClientPlant::requestAssemblies(Assemblies& assemblies)
{
	if (m_tcpComm.connected())
	{
		HeaderPacketComm header;
		PacketComm packetRequest;
		packetRequest.m_header.m_command = Command::GET_ASSEMBLIES;

		std::unique_ptr<char[]> packetTCP = packetRequest.packing();
		m_tcpComm.send(packetTCP.get(), packetRequest.size());

		PacketComm packetResponse = m_lpResponsePacket->get_response(packetRequest);

		if ((packetResponse.m_header.m_idResponse == packetRequest.m_header.m_idResponse) && packetResponse.sizeContent() > 0)
		{
			try
			{
				json parseJSON = json::parse(packetResponse.m_lpContent);
				assemblies.setAssemblies(parseJSON.at("assemblies"));
			}
			catch (std::exception e)
			{
				LOGGER_ERROR("Response Packet Client", e.what());
			}
		}

	}

	return assemblies.getDictAssemblies().size() > 0;
}