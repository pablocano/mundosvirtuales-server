#pragma once

#include "TcpComm.h"
#include "../PacketComm.h"
#include "../Queue.h"
#include "../Runnable.h"
#include "../ResponsePacket.h"

class ClientTCP : public Runnable, public std::enable_shared_from_this<ClientTCP>
{
protected:
	SocketClientTcp m_tcpComm;
	std::function<void(std::shared_ptr<ClientTCP>)> m_closeClientFn;
	std::shared_ptr<ResponsePacket> m_lpResponsePacket;

	void run();

public:
	ClientTCP(const char* ip, int port, std::shared_ptr<ResponsePacket> m_lpResponsePacket, std::function<void(std::shared_ptr<ClientTCP>)> closeClientFn = nullptr);
	ClientTCP(int socket, std::shared_ptr<ResponsePacket> m_lpResponsePacket, std::function<void(std::shared_ptr<ClientTCP>)> closeClientFn);
	~ClientTCP();
};
