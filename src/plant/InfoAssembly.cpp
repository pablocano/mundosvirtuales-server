#include "InfoAssembly.h"

std::string InfoAssembly::getName() const 
{ 
	return m_name;
}

std::string InfoAssembly::getInfo() const 
{ 
	return m_info;
}

std::string InfoAssembly::getShortInfo() const
{ 
	return m_shortInfo;
}

void InfoAssembly::operator=(const Row& row)
{
	ObjectDB::operator=(row);
	this->m_name		= row.get<std::string>("name");
	this->m_info		= row.get<std::string>("info");
	this->m_shortInfo	= row.get<std::string>("shortinfo");
}

Row InfoAssembly::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData;

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("name", TypeData::DB_STRING));
	fieldData->push_back(FieldData("info", TypeData::DB_STRING));
	fieldData->push_back(FieldData("shortInfo", TypeData::DB_STRING));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<std::string>(this->getName());
	row.addRegisterPerValue<std::string>(this->getInfo());
	row.addRegisterPerValue<std::string>(this->getShortInfo());

	return row;
}

void to_json(json& j, const InfoAssembly& m) {
	j = json{ 
		{ "m_id",			m.getID() },
		{ "m_name",			m.m_name },
		{ "m_info",			m.m_info },
		{ "m_shortInfo",	m.m_shortInfo } };
}

void from_json(const json& j, InfoAssembly& m) {
	m.setID(j.at("m_id").get<int>());
	m.m_name		= j.at("m_name").get<std::string>();
	m.m_info		= j.at("m_info").get<std::string>();
	m.m_shortInfo	= j.at("m_shortInfo").get<std::string>();
}