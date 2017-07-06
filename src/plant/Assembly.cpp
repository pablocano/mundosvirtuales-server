#include "Assembly.h"
#include "../utils/db/Rows.h"
#include "../utils/logger/Logger.h"

Assembly::Assembly()
{
}

Assembly::Assembly(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected, bool canShowInfo) :
	assembly_id(id), name(name), info(info), canShowInfo(canShowInfo), shortInfo(shortInfo), pn(pn), canBeSelected(canBeSelected)
{
}

Assembly::~Assembly()
{
}

Assemblies Assembly::loadAssembliesFromDB(DBAdapter* lpDataBase)
{
	using namespace db;

	Assemblies assemblies;

	try
	{
		Rows rows_assemblies = lpDataBase->query("SELECT machines.machines_id AS machine_id, machines.canshowinfo AS canshowinfo, machines.canbeselected AS canbeselected, machinetranslation.name AS name, machines.part_number AS part_number, modelsversion.path_model AS path_model, modelsversion.color AS color, modelsversion.animated AS animated, modelsversion.material AS material, machinetranslation.info AS info, machinetranslation.shortInfo AS shortinfo FROM machines INNER JOIN models ON (models.model_id = machines.Models_model_id) INNER JOIN modelsversion ON ((models.model_id = modelsversion.Models_model_id) AND (models.current_version = modelsversion.version)) INNER JOIN machinetranslation ON ((machinetranslation.Machines_machines_id = machines.machines_id) AND (machinetranslation.Language_language_id = 1))");

		std::map<int, Assembly> map_machines;
		std::map<int, Parts> map_parts;

		for (auto it = rows_assemblies.begin(); it != rows_assemblies.end(); ++it)
		{
			std::vector<int> machine_ids;
			int part_id = it->get<int>("machine_id");
			std::string path_model = it->get<std::string>("path_model");
			std::string material = "/Game/Materials/ProterMaterials/" + it->get<std::string>("color", "") + it->get<std::string>("material") + "." + it->get<std::string>("color", "") + it->get<std::string>("material");
			std::string info = it->get<std::string>("info");
			std::string shortInfo = it->get<std::string>("shortinfo");
			std::string pn = it->get<std::string>("part_number");
			bool animated = it->get<bool>("animated");
			bool canShowInfo = it->get<bool>("canshowinfo");
			bool canBeSelected = it->get<bool>("canbeselected");

			int n_parts = lpDataBase->countQuery("partsofmachine", "Machines_machines_id = " + std::to_string(part_id));

			if (n_parts <= 0)
			{
				// It's only part
				int n_machines = lpDataBase->countQuery("partsofmachine", "Machines_related_machines_id = " + std::to_string(part_id));

				Part part(part_id, path_model, material, info, shortInfo, pn);
				if (n_machines <= 0)
				{
					// Create Machine with only one part
					Assembly machine(part_id, path_model, info, shortInfo, pn, canBeSelected, canShowInfo);
					machine.parts.push_back(part);
					map_machines[part_id] = machine;
				}
				else
				{
					// Save part
					Rows parent_machines = lpDataBase->query("SELECT machines_machines_id FROM partsofmachine WHERE Machines_related_machines_id = " + std::to_string(part_id));
					for (auto it_machines = parent_machines.begin(); it_machines != parent_machines.end(); ++it_machines)
					{
						map_parts[it_machines->get<int>("machines_machines_id")].push_back(part);
					}
				}
			}
			else
			{
				// It's a machine
				Assembly machine(part_id, path_model, info, shortInfo, pn, canBeSelected, canShowInfo);
				map_machines[part_id] = machine;
			}
		}

		// Append parts in Machine
		for (auto machine_id = map_parts.begin(); machine_id != map_parts.end(); ++machine_id)
			map_machines[machine_id->first].parts = machine_id->second;

		// Added Machines
		for (auto it = map_machines.begin(); it != map_machines.end(); ++it)
			assemblies.push_back(it->second);
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("DBAdapter", e.what());
	}

	return assemblies;
}

void Assembly::loadFromDB(DBAdapter* lpDBAdapter)
{

}

void to_json(json& j, const Assembly& m) {
	j = json{ { "id", m.assembly_id }, {"machineParts", m.parts}, { "name", m.name },{ "info", m.info },{"canShowInfo",m.canShowInfo },{ "shortInfo", m.shortInfo },{"pn", m.pn}, {"canBeSelected", m.canBeSelected} };
}

void from_json(const json& j, Assembly& m) {
	m.assembly_id = j.at("id").get<int>();
	m.parts = j.at("machineParts");
	m.name = j.at("name").get<std::string>();
	m.info = j.at("info").get<std::string>();
	m.canShowInfo = j.at("canShowInfo").get<bool>();
	m.shortInfo = j.at("shortInfo").get<std::string>();
	m.pn = j.at("pn").get<std::string>();
	m.canBeSelected = j.at("canBeSelected").get<bool>();
}



