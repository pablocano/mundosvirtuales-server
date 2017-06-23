#include "utils/SystemCall.h"
#include "ServerPlant.h"

#include <iostream>

int main()
{
	ServerPlant server;
	
	std::cout << "Starting Server: " << SIZE_HEADER_PACKET << std::endl;

	server.start();

	SystemCall::sleep(8 * 60 * 60 * 1000);
	
    server.stop(); // wait until server ends last connection
	
	std::cout << "Press enter to finish" << std::endl;
	std::getchar();

	return 0;
}