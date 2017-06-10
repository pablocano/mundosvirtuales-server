#include "../utils/network/TcpComm.h"
#include "../utils/PacketComm.h"
#include "../utils/SystemCall.h"

#include <iostream>
#include <sstream>

int main()
{
	SocketClientTcp tcpComm("127.0.0.1", PORT_SERVERPLANT);
	
	std::stringstream ss;
	if (tcpComm.connected())
	{
		for (int i = 0; i < 5; ++i)
		{
			HeaderPacketComm header;
			PacketComm packet;
			packet.m_header.m_command = Command::GET_INFO_MACHINE;
			packet.m_header.m_statusComm = StatusServer::NORMAL;
			
			ss.str("");
			ss << "Test - " << i;
			std::string str = ss.str();
			packet.m_lpContent = (char *) str.c_str();

			std::cout << "Send package ..." << std::endl;
			std::unique_ptr<char[]> packetTCP = packet.packing();
			tcpComm.send(packetTCP.get(), packet.size());
		}

		SystemCall::sleep(5 * 1000);

		tcpComm.closeSocket();
	}
	else
	{
		std::cout << "Error connection socket" << std::endl;
	}
	
	std::cout << "\n\n ---end---" << std::endl;
	std::getchar();

	return 0;
}