#include "ClientPlant.h"
#include "utils/platform/SystemCall.h"
#include "utils/logger/Logger.h"

#include <exception>

using json = nlohmann::json;

uint32_t ClientPlant::m_indexPacketRequest = 0;

Assemblies ClientPlant::requestMachines()
{
	Assemblies machines;

	if (m_tcpComm.connected())
	{
		HeaderPacketComm header;
		PacketComm packetRequest;
		packetRequest.m_header.m_command = Command::GET_MACHINES;
		packetRequest.m_header.m_idResponse = ++ClientPlant::m_indexPacketRequest;

		std::unique_ptr<char[]> packetTCP = packetRequest.packing();
		m_tcpComm.send(packetTCP.get(), packetRequest.size());

		PacketComm packetResponse = m_lpResponsePacket->get_response(packetRequest);

		if ((packetResponse.m_header.m_idResponse == packetRequest.m_header.m_idResponse) && packetResponse.sizeContent() > 0)
		{
			try
			{
				json parseJSON = json::parse(packetResponse.m_lpContent);
				machines = parseJSON.at("machines");
			}
			catch (std::exception e)
			{
				LOGGER_ERROR("Response Packet Client", e.what());
			}
		}
			
	}	

	return machines;
}