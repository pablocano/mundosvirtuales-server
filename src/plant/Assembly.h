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
/// Class that contains all the information about an assembly. An assembly is made of sub-assemblies.
/// </summary>
class Assembly : public ObjectDB
{
protected:

	InfoAssembly m_infoAssembly; /* Information container. */
	ModelAssembly m_modelAssembly; /* Information about model. */
	std::string m_pn; /* Part number. */

	ListAssemblyRelations m_assemblyRelations; /* List of assembly relations. */

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Assembly() : ObjectDB(0, "assembly", nullptr), m_infoAssembly(), m_modelAssembly(), m_pn(), m_assemblyRelations() {}

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
	/// Gets part number.
	/// </summary>
	/// <returns>Returns a string with part number.</returns>
	std::string getPN() const;

	/// <summary>
	/// Gets information about this assembly.
	/// </summary>
	/// <returns>Returns an object with information about this assembly.</returns>
	const InfoAssembly& getInfo() const;

	/// <summary>
	/// Gets Model.
	/// </summary>
	/// <returns>Returns an object with model information.</returns>
	const ModelAssembly& getModel() const;

	/// <summary>
	/// Gets list of relations.
	/// </summary>
	/// <returns>Returns list of assembly relations.</returns>
	const ListAssemblyRelations& getRelations() const;

	/// <summary>
	/// Sets identificator this object, it is necessary for working with database.
	/// </summary>
	/// <param name="id">Integer for identificator this object.</param>
	void setID(int id) override;

	/// <summary>
	/// Sets identificator this object, it is necessary for working with database.
	/// </summary>
	/// <param name="row">Row where the identifier is extracted.</param>
	void setID(const Row &row);

	/// <summary>
	/// Load object from database.
	/// </summary>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	bool loadFromDB() override;

	/// <summary>
	/// Sets handler database.
	/// </summary>
	/// <param name="lpDBAdapter">Handler database.</param>
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
	/// Save this object in database. If the object exist, it only gets updated. 
	/// </summary>
	/// <returns>Returns true if the object was saved successfully, false otherwise.</returns>
	bool saveToDB();

public:

	friend void to_json(json& j, const Assembly& m);

	friend void from_json(const json& j, Assembly& m);

	friend class Assemblies;
};

/// <summary>
/// Creates an assembly class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The assembly where to store the information.</param>
void from_json(const json& j, Assembly& m);

/// <summary>
/// Return a json representation of the assembly class.
/// </summary>
/// <param name="j">The json structure where the to store the assembly info</param>
/// <param name="m">The assembly class that is going to be stored.</param>
void to_json(json& j, const Assembly& m);

/// <summary>
/// Dict Assemblies.
/// </summary>
typedef std::map<int, Assembly> DictAssemblies;

/// <summary>
/// Creates an dictionary of assemblies class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The dictionary of assemblies where to store the information.</param>
void from_json(const json& j, DictAssemblies& m);

/// <summary>
/// Return a json representation of the dictionary of assemblies.
/// </summary>
/// <param name="j">The json structure where the to store the assembly info</param>
/// <param name="m">The dictionary of assemblies class that is going to be stored.</param>
void to_json(json &j, const DictAssemblies &m);

/// <summary>
/// Singleton for managed a dictionary of assemblies, this dictionary is loaded from database.
/// </summary>
class Assemblies
{
private:
	DictAssemblies m_dictAssemblies; /* Dictionary of Assemblies. */

public:

	/// <summary>
	/// Gets instance of singleton. 
	/// </summary>
	/// <returns>Returns an instance of singleton.</returns>
	static Assemblies& getInstance()
	{
		static Assemblies instance; // Instance of singleton 
		return instance;
	}
private:

	/// <summary>
	/// Constructor.
	/// </summary>
	Assemblies() : m_dictAssemblies() {}

public:

	// Delete copy constructor and operator equals.
	Assemblies(Assemblies const&) = delete;
	void operator=(Assemblies const&) = delete;

	/// <summary>
	/// Gets dictionary of assemblies.
	/// </summary>
	/// <returns>Returns a dictionary od assemblies.</returns>
	DictAssemblies& getDictAssemblies();

	/// <summary>
	/// Evaluate if an assembly exist in dictionary.
	/// </summary>
	/// <param name="id">Identifier of assembly.</param>
	/// <returns>Returns true if the assembly exist in database, false otherwise.</returns>
	bool existAssembly(int id) const;

	/// <summary>
	/// Operator [].
	/// </summary>
	/// <param name="id">Identifier of assembly.</param>
	/// <returns>Returns an assembly if it exist in dictionary.</returns>
	const Assembly& operator[](int id)
	{
		return m_dictAssemblies[id];
	}

	/// <summary>
	/// Loads dictionary of assemblies from database.
	/// </summary>
	/// <param name="lpDBAdapter">Handler of database.</param>
	void loadDictAssembliesFromDB(DBAdapter* lpDBAdapter);

	/// <summary>
	/// Update dictionary of assemblies from database.
	/// </summary>
	/// <param name="lpDBAdapter">Handler database.</param>
	void updateDictAssembliesFromDB(DBAdapter* lpDBAdapter);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	/// <param name="assemblyComm"></param>
	/// <returns></returns>
	int createAssembly(DBAdapter* lpDBAdapter, const AssemblyComm& assemblyComm);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	/// <param name="assemblyComm"></param>
	void updateAssembly(DBAdapter* lpDBAdapter, const AssemblyComm& assemblyComm);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	/// <param name="assemblyComm"></param>
	/// <returns></returns>
	bool processRelation(DBAdapter* lpDBAdapter, AssemblyComm& assemblyComm);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="j"></param>
	void setAssemblies(json j);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	/// <param name="assembly_id"></param>
	/// <returns></returns>
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

/// <summary>
/// Creates an assemblies class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The assemblies where to store the information.</param>
void from_json(const json& j, Assemblies& m);

/// <summary>
/// Returns a json representation of the assemblies.
/// </summary>
/// <param name="j">The json structure where the to store the assembly info</param>
/// <param name="m">The assemblies class that is going to be stored.</param>
void to_json(json& j, const Assemblies& m);
