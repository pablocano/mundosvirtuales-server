#pragma once

#include "utils/PacketComm.h"
#include "utils/network/TcpComm.h"
#include "utils/Queue.h"
#include "utils/Loader.h"

#include <mutex>
#include <condition_variable>
#include "utils/Runnable.h"

using namespace std;

class ServerPlant;

class CommClient : public Runnable, public std::enable_shared_from_this<CommClient>
{
protected:
	SocketClientTcp m_tcpComm;
	Queue<std::shared_ptr<CommClient>>* m_lpQueue;
	ServerPlant* m_lpServerPlant;

	void run();
	
public:
	CommClient(int socket, ServerPlant* lpServerPlant, Queue<std::shared_ptr<CommClient>>* lpQueue);
	~CommClient();
};

class ServerPlant : public Runnable
{
protected:
	Loader m_loader;
	SocketServerTCP* m_lpTcpServer;
	std::mutex m_mutexLoader;
	std::condition_variable m_condvLoader;
	
	std::thread m_thServer;
	
	Queue<std::shared_ptr<CommClient>> m_queueClients;

	void run();

	void closeAllClients();

	std::unique_ptr<PacketComm> processPacket(PacketComm packet, SocketClientTcp& tcpComm);

	friend class CommClient;
	
public:
	ServerPlant();
	~ServerPlant();

	bool exist_client();
};
