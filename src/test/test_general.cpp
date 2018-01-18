#include "plant/procedures/Procedure.h"
#include "../utils/logger/Logger.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <functional>

using namespace std;

int main()
{
	Procedure p("filterchange.xml");

	const Phase* phase = p.CurrentPhase();

	LOGGER_LOG("Test General", "Press enter to finish");
	std::getchar();

	return 0;
}