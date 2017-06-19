#include "../utils/network/TcpComm.h"
#include "../utils/PacketComm.h"
#include "../utils/SystemCall.h"
#include "../utils/network/ClientTCP.h"
#include "../ClientPlant.h"

#include "../utils/Concurrency.h"

#include <iostream>
#include <sstream>


void test_server()
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
}

void test_concurrency()
{
	static Concurrency con(
		[]() { for (int i = 0; i < 100; i++) { std::cout << "."; SystemCall::sleep(50); } },
		[]() { std::cout << "\nFin Thread" << std::endl; },
		1000);

	con.exec();

	SystemCall::sleep(3000);

	std::cout << "Finish test" << std::endl;
}

int main()
{
	test_concurrency();

	std::cout << "Press enter to finish" << std::endl;
	std::getchar();
	
	return 0;
}