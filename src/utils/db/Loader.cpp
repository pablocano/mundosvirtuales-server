#include "Loader.h"

#include <iostream>

Loader::Loader(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	m_dataBase(_db_name, _db_user, _db_host, _db_port, _db_password, _db_engine)
{

}

Machines Loader::load_machines()
{
	Machines machines;

	try
	{
		Rows rows_machines = m_dataBase.query("SELECT machines.machines_id AS machine_id, machines.canshowinfo AS canshowinfo, machines.canbeselected AS canbeselected, machinetranslation.name AS name, machines.part_number AS part_number, modelsversion.path_model AS path_model, modelsversion.color AS color, modelsversion.animated AS animated, modelsversion.material AS material, machinetranslation.info AS info, machinetranslation.shortInfo AS shortinfo FROM machines INNER JOIN models ON (models.id_model = machines.Models_id_model) INNER JOIN modelsversion ON ((models.id_model = modelsversion.Models_id_model) AND (models.current_version = modelsversion.version)) INNER JOIN machinetranslation ON ((machinetranslation.Machines_machines_id = machines.machines_id) AND (machinetranslation.Language_language_id = 1))");

		std::map<int, Machine> map_machines;
		std::map<int, MachineParts> map_parts;

		for (auto it = rows_machines.begin(); it != rows_machines.end(); ++it)
		{
			std::vector<int> machine_ids;
			int part_id				= it->get<int>("machine_id");
			std::string path_model	= it->get<std::string>("path_model");
			std::string material	= "/Game/Materials/ProterMaterials/" + it->get<std::string>("color", "") + it->get<std::string>("material") + "." + it->get<std::string>("color", "") + it->get<std::string>("material");
			std::string info		= it->get<std::string>("info");
			std::string shortInfo	= it->get<std::string>("shortinfo");
			std::string pn			= it->get<std::string>("part_number");
			bool animated			= it->get<bool>("animated");
			bool canShowInfo		= it->get<bool>("canshowinfo");
			bool canBeSelected		= it->get<bool>("canbeselected");

			int n_parts = m_dataBase.countQuery("partsofmachine", "Machines_machines_id = " +std::to_string(part_id));

			if (n_parts <= 0)
			{
				// It's only part
				int n_machines = m_dataBase.countQuery("partsofmachine", "Machines_related_machines_id = " + std::to_string(part_id));

				MachinePart part(part_id, path_model, material, info, shortInfo, pn);
				if (n_machines <= 0)
				{
					// Create Machine with only one part
					Machine machine(part_id, path_model, info, shortInfo, pn, canBeSelected, canShowInfo);
					machine.machineParts.push_back(part);
					map_machines[part_id] = machine;
				}
				else
				{
					// Save part
					Rows parent_machines = m_dataBase.query("SELECT machines_machines_id FROM partsofmachine WHERE Machines_related_machines_id = " + std::to_string(part_id));
					for (auto it_machines = parent_machines.begin(); it_machines != parent_machines.end(); ++it_machines)
					{
						map_parts[it_machines->get<int>("machines_machines_id")].push_back(part);
					}
				}
			}
			else
			{
				// It's a machine
				Machine machine(part_id, path_model, info, shortInfo, pn, canBeSelected, canShowInfo);
				map_machines[part_id] = machine;
			}
		}

		// Append parts in Machine
		for (auto machine_id = map_parts.begin(); machine_id != map_parts.end(); ++machine_id)
			map_machines[machine_id->first].machineParts = machine_id->second;

		// Added Machines
		for (auto it = map_machines.begin(); it != map_machines.end(); ++it)
			machines.push_back(it->second);
	}
	catch (const std::exception &e)
	{
		std::stringstream strErr;
		strErr << "Error: " << e.what() << std::endl;
		std::cerr << strErr.str();
		// strError = strErr.str();
	}

	return machines;
}
