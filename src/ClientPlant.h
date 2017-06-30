#pragma once

#include "utils/communication/PacketComm.h"
#include "utils/communication/ResponsePacketClient.h"
#include "utils/db/Loader.h"
#include "utils/network/TcpComm.h"
#include "utils/network/ClientTCP.h"

/// <summary>
/// Class allows sending requests and responding packets to the server.  
/// </summary>
class ClientPlant : public ClientTCP
{
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="ip">IP or DNS remote host.</param>
	/// <param name="closeClientFn">Callback function when the communication finishes.</param>
	ClientPlant(std::string ip, std::function<void(std::shared_ptr<ClientTCP>)> closeClientFn = nullptr) :
		ClientTCP(ip.c_str(), PORT_SERVERPLANT, std::shared_ptr<ResponsePacketClientPlant>(new ResponsePacketClientPlant()), closeClientFn) {}

	/// <summary>
	/// Gets machines, this function makes request to server.
	/// </summary>
	/// <returns>Returns machines.</returns>
	Assemblies requestMachines();

	static uint32_t m_indexPacketRequest; /* Count request packet. */
};
