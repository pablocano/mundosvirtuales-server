#pragma once

#include "utils/PacketComm.h"
#include "utils/Loader.h"
#include "utils/network/TcpComm.h"
#include "utils/network/ClientTCP.h"
#include "utils/ResponsePacketClient.h"

class ClientPlant : public ClientTCP
{
public:
	ClientPlant(std::string ip, std::function<void(std::shared_ptr<ClientTCP>)> closeClientFn = nullptr) :
		ClientTCP(ip.c_str(), PORT_SERVERPLANT, std::shared_ptr<ResponsePacketClientPlant>(new ResponsePacketClientPlant()), closeClientFn) {}

	Machines requestMachines();

	static uint32_t m_indexPacketRequest;
};
