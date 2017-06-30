#include "utils/platform/SystemCall.h"
#include "utils/logger/Logger.h"
#include "ServerPlant.h"

#include <iostream>

int main()
{
	ServerPlant server;
	
	LOGGER_LOG("Server", "Starting Server");

	server.start();

	SystemCall::sleep(8 * 60 * 60 * 1000);
	
    server.stop(); // wait until server ends last connection

	LOGGER_LOG("Server", "Press enter to finish");
	
	std::getchar();

	return 0;
}