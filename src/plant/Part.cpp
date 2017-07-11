#include "Part.h"


using namespace db;

Part::Part()
{
}

Part::Part(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn) :
	part_id(id), name(name), material(material), info(info), pn(pn), shortInfo(shortInfo), animated(false)
{
}

Part::~Part()
{
}

bool Part::loadFromDB(DBAdapter* lpDBAdapter, int id)
{
	try
	{
		Rows rows = lpDBAdapter->query("SELECT machines.machines_id AS machine_id, machines.canshowinfo AS canshowinfo, machines.canbeselected AS canbeselected, machinetranslation.name AS name, machines.part_number AS part_number, modelsversion.path_model AS path_model, modelsversion.color AS color, modelsversion.animated AS animated, modelsversion.material AS material, machinetranslation.info AS info, machinetranslation.shortInfo AS shortinfo FROM machines INNER JOIN models ON (models.model_id = machines.Models_model_id) INNER JOIN modelsversion ON ((models.model_id = modelsversion.Models_model_id) AND (models.current_version = modelsversion.version)) INNER JOIN machinetranslation ON ((machinetranslation.Machines_machines_id = machines.machines_id) AND (machinetranslation.Language_language_id = 1) AND (machines.machines_id = " + std::to_string(id) + "))");

		if (rows.size())
		{
			Row row = rows.front();
			*this = row;
			return true;
		}
		else
			return false;
	}
	catch (const std::exception& e)
	{
		LOGGER_ERROR("Part", e.what());
		return false;
	}
}

bool Part::saveToDB(DBAdapter* lpDBAdapter)
{
	try
	{
		return lpDBAdapter->insert("machines", this->getRow());
	}
	catch (const std::exception& e)
	{
		LOGGER_ERROR("Part", e.what());
		return false;
	}
}

bool Part::updateToDB(DBAdapter * lpDBAdapter)
{
	try
	{
		return true;
	}
	catch (const std::exception& e)
	{
		LOGGER_ERROR("Part", e.what());
		return false;
	}
}

void Part::operator=(const Row& row)
{
	part_id = row.get<int>("machine_id");
	this->name = row.get<std::string>("path_model");
	material = "/Game/Materials/ProterMaterials/" + row.get<std::string>("color", "") + row.get<std::string>("material") + "." + row.get<std::string>("color", "") + row.get<std::string>("material");
	info = row.get<std::string>("info");
	shortInfo = row.get<std::string>("shortinfo");
	pn = row.get<std::string>("part_number");
	animated = row.get<bool>("animated", false);
}

Row Part::getRow()
{
	Row row;
	Fields fieldData;

	fieldData.push_back(FieldData("machines_id", TypeData::DB_INTEGER));
	fieldData.push_back(FieldData("part_number", TypeData::DB_STRING));
	fieldData.push_back(FieldData("canShowInfo", TypeData::DB_INTEGER));
	fieldData.push_back(FieldData("canBeSelected", TypeData::DB_INTEGER));

	row.setFieldData(&fieldData);

	row.addRegisterPerValue<int>(part_id);
	row.addRegisterPerValue<std::string>(pn);
	row.addRegisterPerValue<bool>(true);
	row.addRegisterPerValue<bool>(true);

	return row;
}

void to_json(json & j, const Part& m)
{
	j = json{ {"id", m.part_id },{"name", m.name},{"material",m.material},{"info",m.info},{"pn",m.pn},{"shortInfo",m.shortInfo},{"animated",m.animated} };
}

void from_json(const json & j, Part& m)
{
	m.part_id = j.at("id").get<int>();
	m.name = j.at("name").get<std::string>();
	m.material = j.at("material").get<std::string>();
	m.info = j.at("info").get<std::string>();
	m.pn = j.at("pn").get<std::string>();
	m.shortInfo = j.at("shortInfo").get<std::string>();
	m.animated = j.at("animated").get<bool>();
}
