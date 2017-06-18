#include "../utils/network/TcpComm.h"
#include "../utils/PacketComm.h"
#include "../utils/SystemCall.h"
#include "../utils/network/ClientTCP.h"
#include "../ClientPlant.h"

#include <iostream>
#include <sstream>


int main()
{
	ClientPlant client("localhost");
	
	std::cout << "Starting Client" << std::endl;

	client.start();

	Machines machines = client.requestMachines();

	for (Machine machine : machines)
	{
		std::cout << machine.name << std::endl;
	}

	SystemCall::sleep(1000);

	client.stop();

	std::cout << "Press enter to finish" << std::endl;
	std::getchar();

	return 0;
}