#include "AssemblyUtils.h"

using json = nlohmann::json;

std::string AssemblyRelation::CreatePath(const std::string & parentPath) const
{
	return parentPath + (parentPath.empty() ? "" : ",") + std::to_string(m_child_assembly_id) + ":" + std::to_string(m_instance);
}

void AssemblyRelation::setDBAdapter(DBAdapter * lpDBAdapter)
{
	ObjectDB::setDBAdapter(lpDBAdapter);
	this->m_position.setDBAdapter(lpDBAdapter);
}

bool AssemblyRelation::saveToDB()
{
	if (m_position.saveToDB())
	{
		return ObjectDB::saveToDB();
	}
	else
	{
		return false;
	}
}

Row AssemblyRelation::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("parent_assembly_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("child_assembly_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("instance", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("position_entity_id", TypeData::DB_INTEGER));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<int>(this->m_parent_assembly_id);
	row.addRegisterPerValue<int>(this->m_child_assembly_id);
	row.addRegisterPerValue<int>(this->m_instance);
	row.addRegisterPerValue<int>(this->m_position.getID());

	return row;
}

void AssemblyRelation::operator=(const Row& row)
{
	ObjectDB::operator=(row);
	this->m_child_assembly_id = row.get<int>("parent_assembly_id");
	this->m_child_assembly_id = row.get<int>("child_assembly_id");
	this->m_instance = row.get<int>("instance");
	this->m_position.setID(row.get<int>("position_entity_id"));
	this->m_position.loadFromDB();
}

void to_json(json& j, const AssemblyRelation& m)
{
	j = json{
		{ "m_id_assembly",	m.m_child_assembly_id },
		{ "m_id_instance",	m.m_instance },
		{ "m_position",		m.m_position } };
}

void from_json(const json& j, AssemblyRelation& m)
{
	m.m_child_assembly_id = j.at("m_id_assembly").get<int>();
	m.m_instance = j.at("m_id_instance").get<int>();
	m.m_position = j.at("m_position").get<Position>();
}

void to_json(json& j, const AssemblyComm& m)
{
	j = json{
		{ "m_part_number",				m.m_part_number },
		{ "m_name",						m.m_name },
		{ "m_filename",					m.m_filename },
		{ "m_id_assembly",				m.m_id_assembly },
		{ "m_version",					m.m_version },
		{ "m_listAssemblyRelations",	m.m_listAssemblyRelations } };
}

void from_json(const json& j, AssemblyComm& m)
{
	m.m_part_number				= j.at("m_part_number").get<std::string>();
	m.m_name					= j.at("m_name").get<std::string>();
	m.m_filename				= j.at("m_filename").get<std::string>();
	m.m_id_assembly				= j.at("m_id_assembly").get<int>();
	m.m_version					= j.at("m_version").get<int>();
	m.m_listAssemblyRelations	= j.at("m_listAssemblyRelations").get<ListAssemblyRelations>();
}