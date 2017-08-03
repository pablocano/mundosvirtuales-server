#pragma once

#include "serialization/json.h"
#include "db/ObjectDB.h"
#include "Position.h"

using json = nlohmann::json;

struct AssemblyRelation : public ObjectDB
{
	int m_parent_assembly_id;
	int m_child_assembly_id;
	int m_instance;
	Position m_position;

	AssemblyRelation() : ObjectDB(0, "assembly_relations", nullptr), m_parent_assembly_id(), m_child_assembly_id(), m_instance(), m_position() {}

	AssemblyRelation(const Row& row) : ObjectDB(row) {}

	/// <summary>
	/// Create the path to this relation
	/// </summary>
	/// <param name="parentPath">The path to the parent.</param>
	/// <returns></returns>
	std::string CreatePath(const std::string& parentPath) const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	void setDBAdapter(DBAdapter* lpDBAdapter);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool saveToDB();

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Returns a row constructed from this object's data.</returns>
	Row getRow() const;

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);
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
