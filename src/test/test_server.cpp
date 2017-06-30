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
	static ClientPlant client("10.0.42.8");

	std::cout << "Starting Client" << std::endl;

	client.start();

	static Assemblies machines;
	
	static Concurrency con([]() -> bool { machines = client.requestMachines(); return machines.size() > 0; },
		[]() { std::cout << "Machines:" << std::endl; for (Assembly machine : machines) std::cout << machine.name << std::endl; }, 100);

	std::cout << "Post concurrency" << std::endl;
	
	SystemCall::sleep(3000);

	std::cout << "Pre sleep" << std::endl;
}

void test_concurrency()
{
	Concurrency con(
		[]() -> bool { int i = 0;  std::cout << "."; SystemCall::sleep(50); return ++i > 1000; },
		[]() { std::cout << "\nFin Thread" << std::endl; },
		2000);

	SystemCall::sleep(3000);

	std::cout << "Finish test" << std::endl;
}

int main()
{
	test_server();

	std::cout << "Press enter to finish" << std::endl;
	std::getchar();
	
	return 0;
}