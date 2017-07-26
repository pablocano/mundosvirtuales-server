#include "AssemblyUtils.h"

using json = nlohmann::json;

void to_json(json& j, const AssemblyRelation& m)
{
	j = json{ 
		{ "m_id_assembly",	m.m_id_assembly },
		{ "m_id_instance",	m.m_id_assembly },
		{ "m_position",		m.m_position } };
}

void from_json(const json& j, AssemblyRelation& m)
{
	m.m_id_assembly		= j.at("m_id_assembly").get<int>();
	m.m_id_instance		= j.at("m_id_instance").get<int>();
	m.m_position		= j.at("m_position").get<Position>();
}

void to_json(json& j, const AssemblyComm& m)
{
	j = json{ 
		{ "m_part_number",				m.m_part_number },
		{ "m_name",						m.m_name },
		{ "m_id_assembly",				m.m_id_assembly },
		{ "m_version",					m.m_version },
		{ "m_listAssemblyRelations",	m.m_listAssemblyRelations } };
}

void from_json(const json& j, AssemblyComm& m)
{
	m.m_part_number				= j.at("m_part_number").get<std::string>();
	m.m_name					= j.at("m_name").get<std::string>();
	m.m_id_assembly				= j.at("m_id_assembly").get<int>();
	m.m_version					= j.at("m_version").get<int>();
	m.m_listAssemblyRelations	= j.at("m_listAssemblyRelations");
}