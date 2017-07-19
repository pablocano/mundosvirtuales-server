#include "Assembly.h"
#include "../utils/db/Rows.h"
#include "../utils/logger/Logger.h"

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

			int n_parts = lpDataBase->countQuery("partsofmachine", "Machines_machines_id = " + std::to_string(part_id));

			if (n_parts <= 0)
			{
				// It's only part
				int n_machines = lpDataBase->countQuery("partsofmachine", "Machines_related_machines_id = " + std::to_string(part_id));

				Part part(*it);
				if (n_machines <= 0)
				{
					// Create Machine with only one part
					Assembly machine(*it);
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
				Assembly machine(*it);
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

void Assembly::operator=(const Row& row)
{
	m_id = row.get<int>("machine_id");
	canBeSelected = row.get<bool>("canbeselected");
	canShowInfo = row.get<bool>("canshowinfo");
	infoAssembly = row;
}

Row Assembly::getRow() const
{
	return Row();
}

void to_json(json& j, const Assembly& m) {
	j = json{ { "id", m.m_id }, {"machineParts", m.parts}, {"canShowInfo",m.canShowInfo }, {"canBeSelected", m.canBeSelected}, { "infoAssembly", m.infoAssembly } };
}

void from_json(const json& j, Assembly& m) {
	m.m_id = j.at("id").get<int>();
	m.parts = j.at("machineParts");
	m.canShowInfo = j.at("canShowInfo").get<bool>();
	m.canBeSelected = j.at("canBeSelected").get<bool>();
	m.infoAssembly = j.at("infoAssembly");
}

void SubAssembly::operator=(const Row & row)
{

}

Row SubAssembly::getRow() const
{
	return Row();
}
