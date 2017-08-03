#include "InfoAssembly.h"

std::string InfoAssembly::getName() const
{
	return m_name;
}

void InfoAssembly::setName(std::string name)
{
	m_name = name;
}

std::string InfoAssembly::getInfo() const
{
	return m_info;
}

std::string InfoAssembly::getShortInfo() const
{
	return m_shortInfo;
}

int InfoAssembly::getAssemblyID() const
{
	return m_assembly_id;
}

void InfoAssembly::setAssemblyID(int id)
{
	m_assembly_id = id;
}

void InfoAssembly::operator=(const Row& row)
{
	ObjectDB::operator=(row);
	this->m_name = row.get<std::string>("name");
	this->m_info = row.get<std::string>("info");
	this->m_shortInfo = row.get<std::string>("shortinfo");
	this->m_language_id = row.get<int>("language_id");
	this->m_assembly_id = row.get<int>("assembly_id");
}

Row InfoAssembly::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("name", TypeData::DB_STRING));
	fieldData->push_back(FieldData("info", TypeData::DB_STRING));
	fieldData->push_back(FieldData("shortInfo", TypeData::DB_STRING));
	fieldData->push_back(FieldData("language_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("assembly_id", TypeData::DB_INTEGER));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<std::string>(this->m_name);
	row.addRegisterPerValue<std::string>(this->m_info);
	row.addRegisterPerValue<std::string>(this->m_shortInfo);
	row.addRegisterPerValue<int>(this->m_language_id);
	row.addRegisterPerValue<int>(this->m_assembly_id);

	return row;
}

void to_json(json& j, const InfoAssembly& m) {
	j = json{
		{ "m_id",			m.getID() },
		{ "m_name",			m.m_name },
		{ "m_info",			m.m_info },
		{ "m_shortInfo",	m.m_shortInfo },
		{ "m_language_id",	m.m_language_id },
		{ "m_assembly_id",	m.m_assembly_id } };
}

void from_json(const json& j, InfoAssembly& m) {
	m.setID(j.at("m_id").get<int>());
	m.m_name = j.at("m_name").get<std::string>();
	m.m_info = j.at("m_info").get<std::string>();
	m.m_shortInfo = j.at("m_shortInfo").get<std::string>();
	m.m_language_id = j.at("m_language_id").get<int>();
	m.m_assembly_id = j.at("m_assembly_id").get<int>();
}