#include "Loader.h"

#include <iostream>
#include <sstream>

#include "soci/soci.h"
#include "soci/postgresql/soci-postgresql.h"


Loader::Loader(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	db_name(_db_name), db_user(_db_user), db_host(_db_host), db_port(_db_port), db_password(_db_password), db_engine(_db_engine), 
	current_language_id(1), // Default language
	strError("")
{

}

std::string Loader::get_str_connection()
{
	std::stringstream ss;
	
	ss.str("");
	if (!db_host.empty())
	{
		ss << "host=" << db_host << " ";
		ss << "port=" << db_port << " ";
	}
	ss << "dbname=" << db_name << " ";
	ss << "user=" << db_user;
	if (!db_password.empty())
		ss << " password=" << db_password;

	return ss.str();
}

void Loader::set_language(std::string lang)
{
	current_language_id = 1; // TODO: select language from lang argument
}

std::string Loader::get_text_from_path(std::string path_file)
{
	return path_file; // TODO: this function must return the content of file.
}

Machines Loader::load_machines()
{
	Machines machines;

	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);

		soci::rowset<soci::row> rows_machines(
			sql.prepare << "SELECT machines.machines_id AS machine_id, machinetranslation.name AS name, machines.part_number AS part_number, models.path_model AS path_model, models.color AS color, models.animated AS animated, models.material AS material, machinetranslation.info AS info, machinetranslation.shortInfo AS shortinfo FROM machines INNER JOIN models ON (models.models_id = machines.Models_models_id) INNER JOIN machinetranslation ON ((machinetranslation.Machines_machines_id = machines.machines_id) AND (machinetranslation.Language_language_id = 1))");

		std::map<int, Machine> map_machines;
		std::map<int, MachineParts> map_parts;

		std::stringstream strSQL;

		for (auto it = rows_machines.begin(); it != rows_machines.end(); ++it)
		{
			std::vector<int> machine_ids;
			int part_id				= it->get<int>("machine_id");
			std::string path_model	= it->get<std::string>("path_model");
			std::string material	= "/Game/Materials/ProterMaterials/" + it->get<std::string>("color", "") + it->get<std::string>("material") + "." + it->get<std::string>("color", "") + it->get<std::string>("material");
			std::string info		= it->get<std::string>("info");
			std::string shortInfo	= it->get<std::string>("shortinfo");
			std::string pn			= it->get<std::string>("part_number");
			bool animated			= it->get<int>("animated") != 0;  // TODO: check boolean values

			strSQL.str("");
			strSQL << "SELECT count(*) FROM partsofmachine WHERE Machines_machines_id = " << part_id;
			int n_parts = 0;
			sql << strSQL.str(), soci::into(n_parts);

			if (n_parts <= 0)
			{
				// It's only part
				strSQL.str("");
				strSQL << "SELECT count(*) FROM partsofmachine WHERE Machines_related_machines_id = " << part_id;
				int n_machines = 0;
				sql << strSQL.str(), soci::into(n_machines);
				MachinePart part(part_id, path_model, material, info, shortInfo, pn);
				if (n_machines <= 0)
				{
					// Create Machine with only one part
					Machine machine(part_id, path_model, info, shortInfo, pn);
					machine.machineParts.push_back(part);
					map_machines[part_id] = machine;
				}
				else
				{
					// Save part
					strSQL.str("");
					strSQL << "SELECT machines_machines_id FROM partsofmachine WHERE Machines_related_machines_id = " << part_id;
					std::vector<int> parent_machines(n_machines);
					sql << strSQL.str(), soci::into(parent_machines);
					for (auto it_machines = parent_machines.begin(); it_machines != parent_machines.end(); ++it_machines)
					{
						map_parts[*it_machines].push_back(part);
					}
				}
			}
			else
			{
				// It's a machine
				Machine machine(part_id, path_model, info, shortInfo, pn);
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
		strError = strErr.str();
	}

	return machines;
}
