#include "ServerPlant.h"
#include "utils\PacketComm.h"

using json = nlohmann::json;

CommClient::CommClient(SocketClientTcp TcpComm, Loader* lpLoader, std::mutex* lpMutexLoader,std::condition_variable* lpCondvLoader) : 
	m_tcpComm(TcpComm), m_lpLoader(lpLoader), m_lpmutexLoader(lpMutexLoader), m_lpcondvLoader(lpCondvLoader)
{
	m_isDetach = true;
}

CommClient::~CommClient()
{
	m_tcpComm.closeSocket();
}

void CommClient::processPacket(PacketComm packet)
{
	switch (packet.m_header.m_command)
	{
	case Command::GET_INFO_PLANT:
		std::cerr << "Get info plant" << std::endl;
		break;
	case Command::GET_LIST_MACHINES:
		std::cerr << "Get list machines" << std::endl;
		break;
	case Command::GET_MODEL_MACHINES:
		std::cerr << "Get model machines" << std::endl;
		break;
	case Command::GET_INFO_MACHINE:
		std::cerr << "Get info machine" << std::endl;
		break;
	case Command::GET_MAINTENANCE_MACHINE:
		std::cerr << "Get maintenance" << std::endl;
		break;
	case Command::GET_LIST_SENSORS:
		std::cerr << "Get list sensors" << std::endl;
		break;
	case Command::GET_SENSORS:
		std::cerr << "Get sensors" << std::endl;
		break;
	case Command::CLOSE_CONNECTION:
		std::cerr << "Close Connection" << std::endl;
		break;
	case Command::NONE:
	default:
		std::cerr << "None command" << std::endl;
	}
}

void CommClient::run()
{
	int count_error = 0;
	while (!m_stop)
	{
		HeaderPacketComm header;

		// Validate
		if (m_tcpComm.receive((char *)&header, SIZE_HEADER_PACKET))
		{
			if (header.version == VERSION_PACKET)
			{
				if (header.m_size > 0)
				{
					char* contentEncrypted = (char *) std::malloc(header.m_size);
					if (m_tcpComm.receive(contentEncrypted, header.m_size, 0))
					{
						PacketComm packet = PacketComm::unpacking(header, contentEncrypted);
						processPacket(packet);
					}
					else
					{
						std::cerr << "Error received Content of Packet" << std::endl;
						count_error++;
					}
					std::free(contentEncrypted);
				}
				else if (header.m_size == 0)
				{
					PacketComm packet = PacketComm::unpacking(header, nullptr);
					processPacket(packet);
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

				char* packetTCP = packet.packing();
				m_tcpComm.send((const char *)&packetTCP, sizeof(packetTCP));

				std::free(packetTCP);
			}
		}
		else
		{
			std::cerr << "Error received packet" << std::endl;
			count_error++;
		}

		if (count_error > 5)
			m_stop = true;
	}

	std::cout << "stop communication client " << m_tcpComm.getInfo() << std::endl;
}

ServerPlant::ServerPlant() : m_loader("proter", "postgres", "10.0.42.8", 5432, "test123")
{
}

ServerPlant::~ServerPlant()
{
	m_lpTcpServer->closeSocket();

	closeAllClients();

	delete m_lpTcpServer;
	try { stop(); }
	catch (...) { /*??*/ }
}

void ServerPlant::run()
{
	m_lpTcpServer = new SocketServerTCP(PORT_SERVERPLANT);
	while (!m_stop || exist_client())
	{
		int socketClient = 0;
		if ((socketClient = m_lpTcpServer->acceptClient()) > 0)
		{
			SocketClientTcp tcpClient(socketClient);
			CommClient* lpClient = new CommClient(tcpClient, &m_loader, &m_mutexLoader, &m_condvLoader);
			
			m_queueClients.add(lpClient);

			lpClient->start();
		}
	}
}

bool ServerPlant::exist_client()
{
	return m_queueClients.size() > 0;
}

void ServerPlant::closeAllClients()
{
	while (exist_client())
	{
		CommClient* lpClient = m_queueClients.remove();
		lpClient->stop();
		delete lpClient;
	}
}