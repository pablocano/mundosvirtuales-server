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

void Assembly::setID(int id)
{
	ObjectDB::setID(id);
	m_infoAssembly.setAssemblyID(id);
}

void Assembly::setID(const Row & row)
{
	ObjectDB::setID(row);
	m_infoAssembly.setAssemblyID(getID());
}

bool Assembly::loadFromDB()
{
	int id = getDBAdapter()->query("SELECT assembly_translation_id as id FROM assembly_translation WHERE ((assembly_id = " + std::to_string(getID()) + ") AND (language_id = 1));").front().get<int>("id"); // TODO: language id fixed
	m_infoAssembly.setID(id);

	return ObjectDB::loadFromDB() && m_infoAssembly.loadFromDB() && m_modelAssembly.loadFromDB();
}

void Assembly::setDBAdapter(DBAdapter * lpDBAdapter)
{
	ObjectDB::setDBAdapter(lpDBAdapter);
	m_infoAssembly.setDBAdapter(lpDBAdapter);
	m_modelAssembly.setDBAdapter(lpDBAdapter);
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

void Assembly::operator=(const AssemblyComm& assemblyComm)
{
	this->m_pn = assemblyComm.m_part_number;
	this->m_infoAssembly.setDBAdapter(getDBAdapter());
	this->m_modelAssembly.setDBAdapter(getDBAdapter());
	this->m_modelAssembly.setPathModel(assemblyComm.m_name);
	this->m_modelAssembly.setVersion(assemblyComm.m_version);
}

Row Assembly::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("part_number", TypeData::DB_STRING));
	fieldData->push_back(FieldData("model_id", TypeData::DB_INTEGER));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<std::string>(this->getPN());
	row.addRegisterPerValue<int>(this->getModel().getID());

	return row;
}

bool Assembly::saveToDB()
{
	if (m_modelAssembly.saveToDB() && ObjectDB::saveToDB())
	{
		m_infoAssembly.setAssemblyID(getID());
		return m_infoAssembly.saveToDB();
	}
	else
		return false;
}

void to_json(json& j, const Assembly& m) {
	j = json{ 
		{"m_id",				m.getID() },
		{"m_pn",				m.getPN() },
		{"m_infoAssembly",		m.getInfo()},
		{"m_modelAssembly",		m.getModel() } };
}

void from_json(const json& j, Assembly& m) {
	m.setID(j.at("m_id").get<int>());
	m.m_pn				= j.at("m_pn");
	m.m_infoAssembly	= j.at("m_infoAssembly");
	m.m_modelAssembly	= j.at("m_modelAssembly");
}

DictAssemblies& Assemblies::getDictAssemblies()
{
	return m_dictAssemblies;
}

bool Assemblies::existAssembly(int id) const
{
	return m_dictAssemblies.find(id) != m_dictAssemblies.end();
}

void Assemblies::loadDictAssembliesFromDB(DBAdapter *lpDBAdapter)
{
	using namespace db;

	try
	{
		Rows rows_assemblies = lpDBAdapter->query("SELECT assembly_id FROM assembly;");

		for (auto it = rows_assemblies.begin(); it != rows_assemblies.end(); ++it)
		{
			int id = it->get<int>("assembly_id");
			Assembly assembly;
			assembly.setDBAdapter(lpDBAdapter);
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

void Assemblies::updateDictAssembliesFromDB(DBAdapter *lpDBAdapter)
{
	loadDictAssembliesFromDB(lpDBAdapter);
}

int Assemblies::createAssembly(DBAdapter* lpDBAdapter, const AssemblyComm& assemblyComm)
{
	updateDictAssembliesFromDB(lpDBAdapter);

	Assembly assembly;
	assembly.setDBAdapter(lpDBAdapter);
	assembly = assemblyComm;
	
	if (assembly.saveToDB() && assembly.getID() > 0)
	{
		Assemblies::getInstance().getDictAssemblies()[assembly.getID()] = assembly;
	}

	return assembly.getID();
}

int Assemblies::updateAssembly(DBAdapter* lpDBAdapter, const AssemblyComm& assemblyComm)
{
	updateDictAssembliesFromDB(lpDBAdapter);

	Assemblies& assemblies = Assemblies::getInstance();

	if (assemblies.existAssembly(assemblyComm.m_id_assembly))
	{
		assemblies.getDictAssemblies()[assemblyComm.m_id_assembly] = assemblyComm; // Copy data
		assemblies.getDictAssemblies()[assemblyComm.m_id_assembly].saveToDB(); // Save data

		ListAssemblyRelations relations = Assemblies::loadRelationFromDB(lpDBAdapter, assemblyComm.m_id_assembly);

		for (auto& commRelation : assemblyComm.m_listAssemblyRelations)
		{
			bool newRelation = true;
			for (auto& relation : relations)
			{
				if (commRelation.m_child_assembly_id == relation.m_child_assembly_id && commRelation.m_instance == relation.m_instance)
				{
					relation.m_position = commRelation.m_position;
					relation.m_position.saveToDB();
					relation.setID(-1);
					newRelation = false;
					break;
				}
			}

			if (newRelation)
			{
				AssemblyRelation newRelation = commRelation;

				newRelation.setDBAdapter(lpDBAdapter);
				newRelation.m_parent_assembly_id = assemblyComm.m_id_assembly;
				
				newRelation.saveToDB();
			}
		}

		for (auto& relation : relations)
		{
			relation.deleteToDB();
		}


		return assemblies.getDictAssemblies()[assemblyComm.m_id_assembly].getID();
	}
	else
	{
		return -1;
	}
}

bool Assemblies::processRelation(DBAdapter* lpDBAdapter, AssemblyComm & assemblyComm)
{
	updateDictAssembliesFromDB(lpDBAdapter);

	for (auto it = assemblyComm.m_listAssemblyRelations.begin(); it != assemblyComm.m_listAssemblyRelations.end() ; ++it)
	{
		it->m_parent_assembly_id = assemblyComm.m_id_assembly;
		it->setDBAdapter(lpDBAdapter);
		if (!it->saveToDB())
		{
			// Rollover
			for (auto it2 = assemblyComm.m_listAssemblyRelations.begin(); it2 != it; ++it2)
			{
				it2->deleteToDB();
			}

			return false;
		}
	}

	return true;
}

void Assemblies::setAssemblies(json j)
{
	from_json(j, *this);
}

ListAssemblyRelations Assemblies::loadRelationFromDB(DBAdapter * lpDBAdapter, int assembly_id)
{
	ListAssemblyRelations listRelations;
	Rows rows = lpDBAdapter->query("SELECT * FROM assembly_relations WHERE parent_assembly_id = " + std::to_string(assembly_id) + ";");

	for (Row& row : rows)
	{
		AssemblyRelation assemblyRelation(row);
		assemblyRelation.setDBAdapter(lpDBAdapter);

		listRelations.push_back(assemblyRelation);
	}

	return listRelations;
}

bool Assemblies::IsConnected(DBAdapter* lpDBAdapter, int id_assembly_start, int id_assembly_end)
{
	// Get all the assemblies that are related with the start assembly
	ListAssemblyRelations relations = loadRelationFromDB(lpDBAdapter, id_assembly_start);

	//Iterate over all relations
	for (const auto& relation : relations)
	{
		// If the current relation is connected to the end assembly, the assemblies are conected
		if (relation.m_child_assembly_id == id_assembly_end)
		{
			return true;
		}
	}

	// Iterate over all relations
	for (const auto& relation : relations)
	{
		// Evaluate if one the childs of the current assembly is connected to the end assembly
		if (IsConnected(lpDBAdapter, relation.m_child_assembly_id, id_assembly_end))
		{
			return true;
		}
	}

	// If there is no connection between the two assemblies, return false
	return false;

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