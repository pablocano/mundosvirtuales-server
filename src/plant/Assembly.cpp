#include "Assembly.h"
#include "../utils/db/Rows.h"
#include "../utils/logger/Logger.h"


Assembly::Assembly() : ObjectDB(0, "assembly", nullptr)
{
}

std::string Assembly::getPN() const
{
	return m_pn;
}

const InfoAssembly& Assembly::getInfo() const
{
	return m_infoAssembly;
}

const ModelAssembly& Assembly::getModel() const
{
	return m_modelAssembly;
}

void Assembly::operator=(const Row& row)
{
	ObjectDB::operator=(row);
	this->m_pn = row.get<std::string>("part_number");
	this->m_infoAssembly.setDBAdapter(getDBAdapter());
	this->m_infoAssembly.loadFromDB(row.get<int>("assembly_translation_id"));
	this->m_modelAssembly.setDBAdapter(getDBAdapter());
	this->m_modelAssembly.loadFromDB(row.get<int>("model_id"));
}

Row Assembly::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData;

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("part_number", TypeData::DB_STRING));
	fieldData->push_back(FieldData("assembly_translation_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("model_id", TypeData::DB_INTEGER));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<std::string>(this->getPN());
	row.addRegisterPerValue<int>(this->getInfo().getID());
	row.addRegisterPerValue<int>(this->getModel().getID());

	return row;
}

std::string Assembly::getIDFieldName() const
{
	return "assembly." + ObjectDB::getIDFieldName();
}

std::string Assembly::getWhere() const
{
	return "(" + ObjectDB::getWhere() + ") AND ( assembly_translation.language_id = 1 )"; // TODO: define default language
}

std::string Assembly::getJoin() const
{
	return "LEFT JOIN assembly_translation ON (assembly.assembly_id = assembly_translation.assembly_id)";
}

std::string Assembly::getFieldsSelect() const
{
	return "assembly.assembly_id as assembly_id, assembly.model_id as model_id, assembly.part_number as part_number, assembly_translation.assembly_translation_id as assembly_translation_id";
}

void to_json(json& j, const Assembly& m) {
	j = json{ 
		{"m_id",				m.getID() },
		{"m_infoAssembly",		m.getInfo()},
		{"m_modelAssembly",		m.getModel() } };
}

void from_json(const json& j, Assembly& m) {
	m.setID(j.at("m_id").get<int>());
	m.m_infoAssembly = j.at("m_infoAssembly");
	m.m_modelAssembly = j.at("m_modelAssembly");
}

DictAssemblies& Assemblies::getDictAssemblies()
{
	return m_dictAssemblies;
}

void Assemblies::updateDictAssembliesFromDB(DBAdapter * lpDataBase)
{
	using namespace db;

	try
	{
		Rows rows_assemblies = lpDataBase->query("SELECT assembly_id FROM assembly;");

		for (auto it = rows_assemblies.begin(); it != rows_assemblies.end(); ++it)
		{
			int id = it->get<int>("assembly_id");
			Assembly assembly;
			assembly.setDBAdapter(lpDataBase);
			assembly.setID(id);
			assembly.loadFromDB();
			m_dictAssemblies[id] = assembly;
		}
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("Assemblies", e.what());
	}

}

void to_json(json &j, const Assemblies &m)
{
	j = json{ { "m_dictAssemblies",	m.m_dictAssemblies } };
}

void from_json(const json& j, Assemblies& m)
{
	m.m_dictAssemblies = j.at("m_dictAssemblies").get<DictAssemblies>();
}

void to_json(json &j, const DictAssemblies &m)
{
	std::map<std::string, Assembly> temp;
	for (auto &i : m)
	{
		temp[std::to_string(i.first)] = i.second;
	}

	j = temp;
}

void from_json(const json& j, DictAssemblies& m)
{
	auto jj = j.get<std::map<std::string, json>>();

	for (auto &i : jj)
	{
		m[std::stoi(i.first)] = i.second.get<Assembly>();
	}
}