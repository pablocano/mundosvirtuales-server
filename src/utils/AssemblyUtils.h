#pragma once

#include "serialization/json.h"
#include "Position.h"

using json = nlohmann::json;

struct AssemblyRelation
{
	int m_id_assembly;
	Position m_position;
	int m_id_instance;

	AssemblyRelation() : m_id_assembly(), m_position() {}
};

void to_json(json& j, const AssemblyRelation& m);

void from_json(const json& j, AssemblyRelation& m);

typedef std::vector<AssemblyRelation> ListAssemblyRelations;

struct AssemblyComm
{
	std::string m_part_number;
	std::string m_name;
	int m_id_assembly;
	int m_version;
	ListAssemblyRelations m_listAssemblyRelations;

	AssemblyComm() : m_part_number(), m_listAssemblyRelations(), m_id_assembly(), m_version() {}
};

void to_json(json& j, const AssemblyComm& m);

void from_json(const json& j, AssemblyComm& m);
