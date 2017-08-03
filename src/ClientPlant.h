#pragma once

#include "utils/communication/PacketComm.h"
#include "utils/communication/ResponsePacketClient.h"
#include "utils/network/TcpComm.h"
#include "utils/network/ClientTCP.h"
#include "plant/StockPlant.h"

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
	/// Gets stock, this function makes request to server.
	/// </summary>
	/// <returns>Returns stock.</returns>
	bool requestPlant(Plant& plant);

	/// <summary>
	///
	/// </summary>
	/// <param name="assemblies"></param>
	/// <returns></returns>
	bool requestAssemblies(Assemblies& assemblies);

	/// <summary>
	///
	/// </summary>
	/// <param name="command"></param>
	/// <param name="s"></param>
	/// <returns></returns>
	std::string request(Command command, std::string s);
};
