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

	static Assemblies &assemblies = Assemblies::getInstance();
	static Plant &plant = Plant::getInstance();
	
	static Concurrency con([]() -> bool { 
		if (client.requestAssemblies(assemblies))
		{
			return client.requestPlant(plant);
		}
		else
			return false;
	},
		[]() { LOGGER_LOG("Response Packet Client", "Plant:"); for (StockPlant machine : plant.getPlant().getSubStock()) LOGGER_LOG("Test Server", machine.getAssembly().getInfo().getName()); }, 100);

	LOGGER_LOG("Test Server", "Post concurrency");
	
	SystemCall::sleep(3000);

	LOGGER_LOG("Test Server", "Pre sleep");
}

bool test_get_assemblies(ClientPlant& client)
{
	LOGGER_LOG("Test Server", "start test GET_ASSEMBLIES");

	static Assemblies &assemblies = Assemblies::getInstance();
	static Plant &plant = Plant::getInstance();

	if (client.requestAssemblies(assemblies))
	{
		if (client.requestPlant(plant))
			return true;
	}

	return false;
}

void test_new_assembly1(ClientPlant& client)
{
	LOGGER_LOG("Test Server", "NEW_ASSEMBLY 1");

	static Assemblies &assemblies = Assemblies::getInstance();
	static Plant &plant = Plant::getInstance();

	AssemblyComm assemblyComm;

	assemblyComm.m_part_number = "PN_TEST_1";
	assemblyComm.m_version = 1;
	assemblyComm.m_name = "/Game/Models/Test1";

	json j_new_assembly = json({ assemblyComm });

	std::string s = client.request(Command::NEW_ASSEMBLY, j_new_assembly.dump());

	LOGGER_LOG("Test Server", s);
}

void test_new_assembly2(ClientPlant& client)
{
	LOGGER_LOG("Test Server", "NEW_ASSEMBLY 2");

	static Assemblies &assemblies = Assemblies::getInstance();
	static Plant &plant = Plant::getInstance();

	AssemblyComm assemblyComm;

	assemblyComm.m_part_number = "PN_TEST_2";
	assemblyComm.m_version = 1;
	assemblyComm.m_name = "/Game/Models/Test2";

	AssemblyRelation assemblyRelation;

	assemblyRelation.m_id_assembly = 1;
	assemblyRelation.m_id_instance = 1;
	assemblyRelation.m_position.m_pos.x = 100;
	assemblyRelation.m_position.m_rot.y = 100;

	assemblyComm.m_listAssemblyRelations.push_back(assemblyRelation);

	json j_new_assembly = json({ assemblyComm });

	std::string s = client.request(Command::NEW_ASSEMBLY, j_new_assembly.dump());

	LOGGER_LOG("Test Server", s);
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
	ClientPlant client("localhost");
	LOGGER_LOG("Test Server", "Starting Client");

	client.start();

	test_new_assembly1(client);

	test_new_assembly2(client);
	//test_server();

	LOGGER_LOG("Test Server", "Press enter to finish");
	std::getchar();
	
	return 0;
}