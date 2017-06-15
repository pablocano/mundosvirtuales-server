#pragma once

#include "TcpComm.h"
#include "ClientTCP.h"
#include "../Runnable.h"
#include "../Queue.h"
#include "../ResponsePacketServer.h"

class ServerTCP : public Runnable
{
protected:
	SocketServerTCP* m_lpTcpServer;
	std::shared_ptr<ResponsePacketServerPlant> m_lpServerPlant;

	Queue<std::shared_ptr<ClientTCP>> m_queueClients;

	void run();

	void remove_client_from_queue(std::shared_ptr<ClientTCP> lpClient);

public:
	ServerTCP(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");
	~ServerTCP();

	bool exist_client();

	void close_all_clients();
};
