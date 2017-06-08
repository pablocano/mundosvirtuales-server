#include "utils/SystemCall.h"
#include "ServerPlant.h"

#include <iostream>

int main()
{
	ServerPlant server;
	
	std::cout << "Se inicia el servidor" << std::endl;

	server.start();

	SystemCall::sleep(10 * 60 * 1000);
	
    server.stop(); // wait until server ends last connection
	
	std::cout << "Presiona cualquier tecla para terminar" << endl;
	std::getchar();

	return 0;
}