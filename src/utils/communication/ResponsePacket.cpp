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