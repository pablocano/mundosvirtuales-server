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
	LOGGER_LOG("Test Server", "GET_ASSEMBLIES");

	static Assemblies &assemblies = Assemblies::getInstance();
	static Plant &plant = Plant::getInstance();

	if (client.requestAssemblies(assemblies))
	{
		if (client.requestPlant(plant))
			return true;
	}

	return false;
}

AssemblyComm newAssemblyComm(std::string pn, std::string name, int version = 1)
{
	AssemblyComm assemblyComm;

	assemblyComm.m_part_number = pn;
	assemblyComm.m_version = version;
	assemblyComm.m_name = "/Game/Models/" + name;

	return assemblyComm;
}

void test_new_assembly(ClientPlant& client, AssemblyComm& assemblyComm)
{
	LOGGER_LOG("Test Server", "NEW_ASSEMBLY");

	static Assemblies &assemblies = Assemblies::getInstance();
	static Plant &plant = Plant::getInstance();

	json j_new_assembly = json(assemblyComm);

	std::string s = client.request(Command::NEW_ASSEMBLY, j_new_assembly.dump());
	json j = json::parse(s.c_str());

	assemblyComm.m_id_assembly = j.at("id").get<int>();
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

	int N = 5;
	std::vector<AssemblyComm> listAssemblyComm;

	for (int i = 0; i < N; ++i)
	{
		AssemblyComm a = newAssemblyComm("PN_" + std::to_string(i), "NAME_" + std::to_string(i));
		test_new_assembly(client, a);
		if (a.m_id_assembly > 0)
			listAssemblyComm.push_back(a);
	}

	for (int i = 1; i < (int)listAssemblyComm.size(); ++i)
	{
		AssemblyRelation r;
		r.m_child_assembly_id = listAssemblyComm.at(i).m_id_assembly;
		r.m_instance = 1;
		r.m_position.m_pos.x = 10.0f * i;
		listAssemblyComm.at(1).m_listAssemblyRelations.push_back(r);
	}

	if (listAssemblyComm.size() > 1)
		test_new_assembly(client, listAssemblyComm.at(1));

	LOGGER_LOG("Test Server", "Press enter to finish");
	std::getchar();
	
	return 0;
}