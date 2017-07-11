#pragma once

#include "../utils/serialization/json.h"
#include "../utils/db/DBAdapter.h"
#include "Part.h"
#include "ObjectPlant.h"
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace db;

class Assembly;

/// <summary>
/// Definition of a vector of assemblies.
/// </summary>
typedef std::vector<Assembly> Assemblies;

/// <summary>
/// Class that contains all the information about an assembly. An assembly is made of sub-assemblies and parts
/// </summary>
class Assembly : public ObjectPlant
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Assembly();

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="id">Unique identifier for this assembly.</param>
	/// <param name="name">The name of the machine shown in the game.</param>
	/// <param name="info">The description of this assembly.</param>
	/// <param name="shortInfo">A little description about the assembly.</param>
	/// <param name="pn">The part number of this assembly.</param>
	/// <param name="canBeSelected">If this assembly can be selected in the virtual environment.</param>
	/// <param name="canShowInfo">If this assembly has some info to shown.</param>
	Assembly(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected = false, bool canShowInfo = false);
	
	/// <summary>
	/// Constructor object from row.
	/// </summary>
	/// <param name="row">Row</param>
	Assembly(const Row& row) : Assembly() { *this = row; }

	/// <summary>
	/// Destructor
	/// </summary>
	~Assembly();

	/// <summary>
	/// Load Assemblies from database.
	/// </summary>
	/// <param name="m_lpDataBase">Pointer to the database handle.</param>
	/// <returns>Returns a vector with all assemblies stored in the database.</returns>
	static Assemblies loadAssembliesFromDB(DBAdapter* m_lpDataBase);

	/// <summary>
	/// Load properties this object from database.
	/// </summary>
	/// <param name="lpDBAdapter">Pointer to the database handle.</param>
	/// <param name="id">Identifier of Assembly in the database.</param>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	bool loadFromDB(DBAdapter* lpDBAdapter, int id);

	/// <summary>
	/// Save Assembly to database.
	/// </summary>
	/// <param name="lpDBAdapter">Pointer to the database handle.</param>
	/// <returns>Returns true if this object was saved successfully, false otherwise.</returns>
	bool saveToDB(DBAdapter* lpDBAdapter);

	/// <summary>
	/// Update Assembly to database.
	/// </summary>
	/// <param name="lpDBAdapter">Pointer to the database handle.</param>
	/// <returns>Returns true if this object was updated successfully, false otherwise.</returns>
	bool updateToDB(DBAdapter* lpDBAdapter);

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Rerturns a row contructed from this object data.</returns>
	Row getRow();

	/// <summary>
	/// All the sub-assemblies of this assembly
	/// </summary>
	std::vector<Assembly> subAssemblies;

	/// <summary>
	/// All the parts that this assembly is made of.
	/// </summary>
	std::vector<Part> parts;

	/// <summary>
	/// The name of the machine
	/// </summary>
	std::string name;

	/// <summary>
	/// If this assembly has inforation to shown.
	/// </summary>
	bool canShowInfo;

	/// <summary>
	/// The descriptions of this assembly.
	/// </summary>
	std::string info;

	/// <summary>
	/// A short description of this assembly.
	/// </summary>
	std::string shortInfo;

	/// <summary>
	/// The part number of this assembly.
	/// </summary>
	std::string pn;

	/// <summary>
	/// If this assembly can be selected in the virtual environment.
	/// </summary>
	bool canBeSelected;

	/// <summary>
	/// The unique identifier of this assembly.
	/// </summary>
	int assembly_id;
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