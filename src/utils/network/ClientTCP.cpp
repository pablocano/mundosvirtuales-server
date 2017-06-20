#include "ClientTCP.h"

#include <iostream>

ClientTCP::ClientTCP(const char* ip, int port, std::shared_ptr<ResponsePacket> lpProcessPacket, std::function<void(std::shared_ptr<ClientTCP>)> closeClientFn, bool isDetach) : Runnable(isDetach),
	m_tcpComm(ip, port), m_lpResponsePacket(lpProcessPacket), m_closeClientFn(closeClientFn)
{
}

ClientTCP::ClientTCP(int socket, std::shared_ptr<ResponsePacket> lpProcessPacket, std::function<void(std::shared_ptr<ClientTCP>)> closeClientFn, bool isDetach) : Runnable(isDetach),
	m_tcpComm(socket), m_lpResponsePacket(lpProcessPacket), m_closeClientFn(closeClientFn)
{
}

ClientTCP::~ClientTCP()
{
	m_tcpComm.closeSocket();
	std::cout << "Delete Thread Communication Client " << m_tcpComm.getInfo() << std::endl;
}

void ClientTCP::run()
{
	int count_error = 0;
	std::cout << "Run Client " << m_tcpComm.getInfo() << std::endl;
	char* lpBuffer = nullptr;
	while (!m_stop)
	{
		HeaderPacketComm header;

		// Validate
		if (m_tcpComm.receive((char *)&header, SIZE_HEADER_PACKET))
		{
			if (header.version == VERSION_PACKET)
			{
				if (header.m_size >= 0)
				{
					char* contentEncrypted = nullptr;

					if (header.m_size)
					{
						contentEncrypted = (char *)std::realloc(lpBuffer, header.m_size);
						lpBuffer = contentEncrypted;
					}

					if (header.m_size == 0 || m_tcpComm.receive(contentEncrypted, header.m_size, 0))
					{
						PacketComm packet = PacketComm::unpacking(header, contentEncrypted);
						std::unique_ptr<PacketComm> packetResponse = m_lpResponsePacket->process_packet(packet, m_tcpComm);
						if (packetResponse.get())
						{
							std::unique_ptr<char[]> packetTCP = packetResponse->packing();
							m_tcpComm.send(packetTCP.get(), packetResponse->size());
						}
					}
					else
					{
						std::cerr << "Error received Content of Packet" << std::endl;
						count_error++;
					}
				}
				else
				{
					std::cerr << "Error received Header of Packet" << std::endl;
					count_error++;
				}
			}
			else
			{
				std::cerr << "Error Version Packet" << std::endl;
				count_error++;

				PacketComm packet;
				packet.m_header.m_command = Command::NONE;
				packet.m_header.m_statusComm = StatusServer::WRONG_VERSION;
				packet.m_lpContent = nullptr;

				std::unique_ptr<char[]> packetTCP = packet.packing();
				m_tcpComm.send(packetTCP.get(), packet.size());
			}
		}
		else
		{
			count_error++;
		}

		if (count_error >= MAX_ERROR_COMM)
			m_stop = true;
	}

	std::cout << "Stop Communication Client " << m_tcpComm.getInfo() << std::endl;
	if (lpBuffer)
		std::free(lpBuffer);
	
	if (m_closeClientFn)
		m_closeClientFn(shared_from_this());
}