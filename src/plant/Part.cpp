#include "Part.h"



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
		return true;
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
}

Row Part::getRow()
{
	return Row();
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
