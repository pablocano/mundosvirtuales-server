#include "ServerPlant.h"
#include "utils\PacketComm.h"

#define MAX_ERROR_COMM 5

using json = nlohmann::json;

CommClient::CommClient(int socket, ServerPlant* lpServerPlant, Queue<std::shared_ptr<CommClient>>* lpQueue) :
	m_tcpComm(socket), m_lpServerPlant(lpServerPlant), m_lpQueue(lpQueue)
{
	m_isDetach = true;
}

CommClient::~CommClient()
{
	m_tcpComm.closeSocket();
	std::cout << "Delete Thread Communication Client " << m_tcpComm.getInfo() << std::endl;
}

void CommClient::run()
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
						std::unique_ptr<PacketComm> packetResponse = m_lpServerPlant->processPacket(packet, m_tcpComm);
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
	m_lpQueue->remove(shared_from_this());
}

ServerPlant::ServerPlant() : m_loader("proter", "postgres", "10.0.42.8", 5432, "test123")
{
	std::cout << "Create Server" << std::endl;
}

ServerPlant::~ServerPlant()
{
	m_lpTcpServer->closeSocket();

	closeAllClients();

	delete m_lpTcpServer;
	try { stop(); }
	catch (...) { /*??*/ }

	std::cout << "Delete Thread Server" << std::endl;
}

void ServerPlant::run()
{
	m_lpTcpServer = new SocketServerTCP(PORT_SERVERPLANT);
	std::cout << "Run Server" << std::endl;
	while (!m_stop || exist_client())
	{
		int socketClient = 0;
		if ((socketClient = m_lpTcpServer->acceptClient()) > 0)
		{
			std::shared_ptr<CommClient> lpClient(new CommClient(socketClient, this, &m_queueClients));
			
			m_queueClients.add(lpClient);

			lpClient->start();
		}
	}
	std::cout << "Stop Server" << std::endl;
}

bool ServerPlant::exist_client()
{
	return m_queueClients.size() > 0;
}

void ServerPlant::closeAllClients()
{
	while (exist_client())
	{
		std::shared_ptr<CommClient> lpClient = m_queueClients.remove();
		lpClient->stop();
	}
}

std::unique_ptr<PacketComm> ServerPlant::processPacket(PacketComm packet, SocketClientTcp& tcpComm)
{
	std::cout << tcpComm.getInfo() << ":" << std::endl;
	switch (packet.m_header.m_command)
	{
	case Command::GET_INFO_PLANT:
		std::cout << "Get info plant" << std::endl;
		break;
	case Command::GET_LIST_MACHINES:
		std::cout << "Get list machines" << std::endl;
		break;
	case Command::GET_MODEL_MACHINES:
		std::cout << "Get model machines" << std::endl;
		break;
	case Command::GET_INFO_MACHINE:
		std::cout << "Get info machine" << std::endl;
		break;
	case Command::GET_MAINTENANCE_MACHINE:
		std::cout << "Get maintenance" << std::endl;
		break;
	case Command::GET_LIST_SENSORS:
		std::cout << "Get list sensors" << std::endl;
		break;
	case Command::GET_SENSORS:
		std::cout << "Get sensors" << std::endl;
		break;
	case Command::CLOSE_CONNECTION:
		std::cout << "Close Connection" << std::endl;
		break;
	case Command::NONE:
	default:
		std::cout << "None command" << std::endl;
	}

	if (packet.sizeContent() > 0)
		std::cout << packet.m_lpContent << std::endl;

	return nullptr;
}
