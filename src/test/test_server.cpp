#include "../utils/communication/PacketComm.h"
#include "../utils/concurrency/Concurrency.h"
#include "../utils/network/ClientTCP.h"
#include "../utils/platform/SystemCall.h"
#include "../ClientPlant.h"
#include "../utils/logger/Logger.h"

#include <iostream>
#include <sstream>


void test_server()
{
	static ClientPlant client("localhost");

	LOGGER_LOG("Test Server", "Starting Client");

	client.start();

	static StockPlant plant;
	
	static Concurrency con([]() -> bool { plant = client.requestStockPlant(); return plant.isEnable(); },
		[]() { LOGGER_LOG("Response Packet Client", "Machines:"); for (StockPlant machine : plant.getSubStock()) LOGGER_LOG("Test Server", machine.getAssembly().getInfo().getName()); }, 100);

	LOGGER_LOG("Test Server", "Post concurrency");
	
	SystemCall::sleep(3000);

	LOGGER_LOG("Test Server", "Pre sleep");
}

void test_concurrency()
{
	Concurrency con(
		[]() -> bool { int i = 0;  std::cout << "."; SystemCall::sleep(50); return ++i > 1000; },
		[]() { LOGGER_LOG("Test Server", "\nFin Thread"); },
		2000);

	SystemCall::sleep(3000);

	LOGGER_LOG("Test Server", "Finish test");
}

int main()
{
	test_server();

	LOGGER_LOG("Test Server", "Press enter to finish");
	std::getchar();
	
	return 0;
}