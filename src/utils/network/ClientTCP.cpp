#include "ClientTCP.h"
#include "../logger/Logger.h"
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
	LOGGER_DEBUG("Client TCP", "Delete Thread Communication Client " + m_tcpComm.getInfo());
}

void ClientTCP::run()
{
	int count_error = 0;
	LOGGER_DEBUG("Client TCP", "Run Client " + m_tcpComm.getInfo());
	char* lpBuffer = nullptr;
	while (!m_stop)
	{
		HeaderPacketComm header;

		try
		{
			// Read header packet.
			if (m_tcpComm.receive((char *)&header, SIZE_HEADER_PACKET))
			{
				// Validate version packet.
				if (header.version == VERSION_PACKET)
				{
					// Validate size payload.
					if (header.m_size >= 0)
					{
						char* contentEncrypted = nullptr;

						if (header.m_size)
						{
							// Reserve memory of encrypted content.
							contentEncrypted = (char *)std::realloc(lpBuffer, header.m_size);
							lpBuffer = contentEncrypted;
						}

						if (header.m_size == 0 || m_tcpComm.receive(contentEncrypted, header.m_size, 0))
						{
							// Unpacking payload of packet. 
							PacketComm packet = PacketComm::unpacking(header, contentEncrypted);
							// Process packet, returns a response if it necessary.
							std::unique_ptr<PacketComm> packetResponse = m_lpResponsePacket->process_packet(packet, m_tcpComm);
							
							if (packetResponse.get())
							{
								// Response packaging.
								std::unique_ptr<char[]> packetTCP = packetResponse->packing();
								// Send response.
								m_tcpComm.send(packetTCP.get(), packetResponse->size());
							}
						}
						else
						{
							LOGGER_ERROR("Client TCP", "Error received Content of Packet");
							count_error++;
						}
					}
					else
					{
						LOGGER_ERROR("Client TCP", "Error received Header of Packet");
						count_error++;
					}
				}
				else
				{
					// Return wrong version response.
					LOGGER_ERROR("Client TCP", "Error Version Packet");
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
		}
		catch (const std::exception& e)
		{
			LOGGER_ERROR("ClientTCP", e.what());
			count_error++;
		}

		if (count_error >= MAX_ERROR_COMM)
			m_stop = true;
	}

	LOGGER_DEBUG("Client TCP", "Stop Communication Client " + m_tcpComm.getInfo());
	if (lpBuffer)
		std::free(lpBuffer); // Free buffer memory.

	// This function is called function when the server was closed.
	if (m_closeClientFn)
		m_closeClientFn(shared_from_this());
}