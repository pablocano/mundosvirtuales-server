#include "ServerTCP.h"

ServerTCP::ServerTCP(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	m_lpServerPlant( new ResponsePacketServerPlant(_db_name, _db_user, _db_host, _db_port, _db_password, _db_engine))
{
	std::cout << "Create Server" << std::endl;
}

ServerTCP::~ServerTCP()
{
	m_lpTcpServer->closeSocket();

	close_all_clients();

	delete m_lpTcpServer;
	try { stop(); }
	catch (...) { /*??*/ }

	std::cout << "Delete Thread Server" << std::endl;
}

void ServerTCP::run()
{
	m_lpTcpServer = new SocketServerTCP(PORT_SERVERPLANT);
	std::cout << "Run Server" << std::endl;
	while (!m_stop || exist_client())
	{
		int socketClient = 0;
		if ((socketClient = m_lpTcpServer->acceptClient()) > 0)
		{
			std::shared_ptr<ClientTCP> lpClient(new ClientTCP(socketClient,
				std::dynamic_pointer_cast<ResponsePacket>(m_lpServerPlant), std::bind(&ServerTCP::remove_client_from_queue, this, std::placeholders::_1)));

			m_queueClients.add(lpClient);

			lpClient->start();
		}
	}
	std::cout << "Stop Server" << std::endl;
}

bool ServerTCP::exist_client()
{
	return m_queueClients.size() > 0;
}

void ServerTCP::close_all_clients()
{
	while (exist_client())
	{
		std::shared_ptr<ClientTCP> lpClient = m_queueClients.remove();
		lpClient->stop();
	}
}

void ServerTCP::remove_client_from_queue(std::shared_ptr<ClientTCP> lpClient)
{
	m_queueClients.remove(lpClient);
}
