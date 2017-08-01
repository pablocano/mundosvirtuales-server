#pragma once

#include "serialization/json.h"
#include "Position.h"

using json = nlohmann::json;

struct AssemblyRelation
{
	int m_id_assembly; // TODO: change to m_assembly_id
	Position m_position;
	int m_id_instance;

	AssemblyRelation() : m_id_assembly(), m_position() {}

	/// <summary>
	/// Create the path to this relation
	/// </summary>
	/// <param name="parentPath">The path to the parent</param>
	/// <returns></returns>
	std::string CreatePath(const std::string& parentPath) const
	{
		return parentPath + (parentPath.empty() ? "" : ",") + std::to_string(m_id_assembly) + ":" + std::to_string(m_id_instance);
	}
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

	bool isOnlyAssembly() { return m_listAssemblyRelations.size() <= 0; }
};

void to_json(json& j, const AssemblyComm& m);

void from_json(const json& j, AssemblyComm& m);
