#include "../utils/network/TcpComm.h"
#include "../utils/PacketComm.h"
#include "../utils/SystemCall.h"

#include <iostream>
#include <string.h>

int main()
{
	SocketClientTcp tcpComm("127.0.0.1", PORT_SERVERPLANT);
	
	if (tcpComm.connected())
	{
		HeaderPacketComm header;
		PacketComm packet;
		packet.m_header.m_command = Command::NONE;
		packet.m_header.m_statusComm = StatusServer::NORMAL;
		packet.m_lpContent = "Hola Mundo!!";

		std::cout << "Send package ..." << std::endl;
		char* packetTCP = packet.packing();
		tcpComm.send(packetTCP, packet.size());
		std::free(packetTCP);

		SystemCall::sleep(1 * 60 * 1000);
	}
	else
	{
		std::cout << "Error connection socket" << std::endl;
	}
	
	std::cout << "\n\n ---end---" << std::endl;
	std::getchar();

	return 0;
}