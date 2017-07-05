#include "ResponsePacket.h"
#include "ResponsePacket.h"

PacketComm ResponsePacket::get_response(PacketComm &request)
{
	std::shared_ptr<PacketComm> response = m_queueResponsePacket.remove(request); // pop response from queue

	if (response)
	{
		return *response;
	}
	else
	{
		return PacketComm();
	}
}

bool ResponsePacket::sendResponse(SocketClientTcp& tcpComm, PacketComm& packet, char* lpContent, StatusServer statusServer)
{
	HeaderPacketComm header;
	PacketComm responsePacket;
	responsePacket.m_header.m_command = Command::RESPONSE_COMMAND;
	responsePacket.m_header.m_idResponse = packet.m_header.m_idResponse;
	responsePacket.m_header.m_statusComm = StatusServer::NORMAL;

	responsePacket.m_lpContent = lpContent;

	std::unique_ptr<char[]> packetTCP = responsePacket.packing();

	return tcpComm.send(packetTCP.get(), responsePacket.size());
}
