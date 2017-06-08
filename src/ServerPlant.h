#pragma once

#include "utils/PacketComm.h"
#include "utils/network/TcpComm.h"
#include "utils/Queue.h"
#include "utils/Loader.h"

#include <mutex>
#include <condition_variable>
#include "utils/Runnable.h"

using namespace std;

class CommClient : public Runnable
{
protected:
	SocketClientTcp m_tcpComm;
	std::mutex* m_lpmutexLoader;
	std::condition_variable* m_lpcondvLoader;
	Loader* m_lpLoader;

	void run();
	
public:
	CommClient(SocketClientTcp TcpComm, Loader* lpLoader, std::mutex* lpMutexLoader, std::condition_variable* lpCondvLoader);
	~CommClient();

	void processPacket(PacketComm packet);
};

class ServerPlant : public Runnable
{
protected:
	Loader m_loader;
	SocketServerTCP* m_lpTcpServer;
	std::mutex m_mutexLoader;
	std::condition_variable m_condvLoader;
	
	std::thread m_thServer;
	
	Queue<CommClient*> m_queueClients;

	void run();

	void closeAllClients();
	
public:
	ServerPlant();
	~ServerPlant();

	bool exist_client();
};
