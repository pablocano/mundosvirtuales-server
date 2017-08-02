#pragma once

#include "../utils/serialization/json.h"
#include "../utils/db/DBAdapter.h"
#include "../utils/db/ObjectDB.h"
#include "../utils/Position.h"
#include "../utils/AssemblyUtils.h"
#include "ModelAssembly.h"
#include "InfoAssembly.h"
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace db;

/// <summary>
/// Class that contains all the information about an assembly. An assembly is made of sub-assemblies and parts
/// </summary>
class Assembly : public ObjectDB
{
protected:

	InfoAssembly m_infoAssembly; /* Information container. */
	ModelAssembly m_modelAssembly; /* Information about model. */
	std::string m_pn; /* Part number. */

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Assembly();
	
	/// <summary>
	/// Constructor object from row.
	/// </summary>
	/// <param name="row">Row</param>
	Assembly(const Row& row) : Assembly() { *this = row; }

	/// <summary>
	/// Destructor
	/// </summary>
	~Assembly() {}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getPN() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const InfoAssembly& getInfo() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const ModelAssembly& getModel() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	void setID(int id);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="row"></param>
	virtual void setID(const Row &row);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool loadFromDB();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	void setDBAdapter(DBAdapter* lpDBAdapter);

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);

	/// <summary>
	/// Operator equals with AssemblyComm.
	/// </summary>
	/// <param name="row">AssemblyComm reference.</param>
	void operator=(const AssemblyComm& assemblyComm);

private:
	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Rerturns a row contructed from this object data.</returns>
	Row getRow() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool saveToDB();

public:

	friend void to_json(json& j, const Assembly& m);

	friend void from_json(const json& j, Assembly& m);

	friend class Assemblies;
};

/// <summary>
/// Return a json representation of the assembly class.
/// </summary>
/// <param name="j">The json structure where the to store the assembly info</param>
/// <param name="m">The assembly class that is going to be stored.</param>
void to_json(json& j, const Assembly& m);

/// <summary>
/// Creates an assembly class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The assembly where to store the information.</param>
void from_json(const json& j, Assembly& m);

/// <summary>
/// Dict Assemblies.
/// </summary>
typedef std::map<int, Assembly> DictAssemblies;

void from_json(const json& j, DictAssemblies& m);

void to_json(json &j, const DictAssemblies &m);

class Assemblies
{
private:
	DictAssemblies m_dictAssemblies;

public:
	static Assemblies& getInstance()
	{
		static Assemblies instance;
		return instance;
	}
private:
	Assemblies() : m_dictAssemblies() {}

public:
	Assemblies(Assemblies const&) = delete;
	void operator=(Assemblies const&) = delete;

	DictAssemblies& getDictAssemblies();

	bool existAssembly(int id) const;

	const Assembly& operator[](int id)
	{
		return m_dictAssemblies[id];
	}

	void loadDictAssembliesFromDB(DBAdapter* lpDBAdapter);

	void updateDictAssembliesFromDB(DBAdapter* lpDBAdapter);

	int createAssembly(DBAdapter* lpDBAdapter, const AssemblyComm& assemblyComm);

	int updateAssembly(DBAdapter* lpDBAdapter, const AssemblyComm& assemblyComm);

	void setAssemblies(json j);

	bool processRelation(DBAdapter* lpDBAdapter, AssemblyComm& assemblyComm);

	int saveRelationToDB(DBAdapter* lpDBAdapter, int parent_id, int child_id, int instance, Position& position);

	static ListAssemblyRelations loadRelationFromDB(DBAdapter* lpDBAdapter, int assembly_id);

	/// <summary>
	/// Evaluate if the start assembly is a parent of the end assembly
	/// </summary>
	/// <param name="lpDBAdapter">The adapter used to connect to the server</param>
	/// <param name="id_assembly_start">The is of the start assembly</param>
	/// <param name="id_assembly_end">The id of the end assembly</param>
	/// <returns></returns>
	static bool IsConnected(DBAdapter* lpDBAdapter, int id_assembly_start, int id_assembly_end);
	
	friend void to_json(json& j, const Assemblies& m);

	friend void from_json(const json& j, Assemblies& m);
};


void to_json(json& j, const Assemblies& m);

void from_json(const json& j, Assemblies& m);