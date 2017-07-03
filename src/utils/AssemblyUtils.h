#include "serialization/json.h"
#include "Position.h"

using json = nlohmann::json;

struct AssemblyRelation
{
	int m_id_assembly_parent;
	int m_id_assembly_child;
	Position m_position;

	AssemblyRelation() : m_id_assembly_parent(), m_id_assembly_child(), m_position() {}
};

void to_json(json& j, const AssemblyRelation& m);

void from_json(const json& j, AssemblyRelation& m);

typedef struct _AssemblyComm
{
	std::string m_part_number;
	std::string m_name;
	int m_id_assembly;
	int m_version;
	std::vector<AssemblyRelation> m_listAssemblyRelations;

	_AssemblyComm() : m_part_number(), m_listAssemblyRelations(), m_id_assembly(), m_version() {}
} AssemblyComm;

void to_json(json& j, const AssemblyComm& m);

void from_json(const json& j, AssemblyComm& m);
