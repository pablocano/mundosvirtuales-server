#include "utils/SystemCall.h"
#include "ServerPlant.h"

#include <iostream>

int main()
{
	ServerPlant server;
	
	std::cout << "Starting Server" << std::endl;

	server.start();

	SystemCall::sleep(10 * 1000);
	
    server.stop(); // wait until server ends last connection
	
	std::cout << "Press enter to finish" << endl;
	std::getchar();

	return 0;
}