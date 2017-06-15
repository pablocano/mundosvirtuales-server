#include "ClientPlant.h"
#include "utils/SystemCall.h"

#include <exception>

using json = nlohmann::json;

uint32_t ClientPlant::m_indexPacketRequest = 0;

Machines ClientPlant::requestMachines()
{
	Machines machines;

	if (m_tcpComm.connected())
	{
		HeaderPacketComm header;
		PacketComm packetRequest;
		packetRequest.m_header.m_command = Command::GET_MACHINES;
		packetRequest.m_header.m_idResponse = ++ClientPlant::m_indexPacketRequest;
		packetRequest.m_lpContent = "Test largo por que si no no funciona";

		std::cout << "Send package ..." << std::endl;
		std::unique_ptr<char[]> packetTCP = packetRequest.packing();
		m_tcpComm.send(packetTCP.get(), packetRequest.size());
		SystemCall::sleep(10);
		PacketComm packetResponse = m_lpResponsePacket->get_response(packetRequest);

		if ((packetResponse.m_header.m_idResponse == packetRequest.m_header.m_idResponse) && packetResponse.sizeContent() > 0)
		{
			try
			{
				auto parseJSON = json::parse(packetResponse.m_lpContent);
				machines = parseJSON.at("machines");
			}
			catch (std::exception e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
			
	}	

	return machines;
}