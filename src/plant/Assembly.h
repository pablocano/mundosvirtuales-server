#pragma once

#include "../utils/serialization/json.h"
#include "../utils/db/DBAdapter.h"
#include "../utils/db/ObjectDB.h"
#include "../utils/Position.h"
#include "ModelAssembly.h"
#include "InfoAssembly.h"
#include "Part.h"
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace db;

class Assembly;

/// <summary>
/// 
/// </summary>
class SubAssembly : public ObjectDB
{
protected:
	
	Position m_position;
	std::shared_ptr<Assembly> m_lpAssembly;

public:

	SubAssembly();

	Position getPosition() const;

	Assembly getAssembly() const;

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Returns a row constructed from this object's data.</returns>
	Row getRow() const;
};

/// <summary>
/// Definition of a vector of subassemblies.
/// </summary>
class SubAssemblies
{
protected:
	typedef std::vector<SubAssembly> m_subAssemblies;

public:
	SubAssemblies();

	void push_back(SubAssembly& subAssembly);

	SubAssembly& front();

	SubAssembly& back();

	std::vector<SubAssembly>::iterator begin();

	std::vector<SubAssembly>::iterator end();

	std::vector<SubAssembly>::const_iterator cbegin() const;

	std::vector<SubAssembly>::const_iterator cend() const;

	int size() const;
};


/// <summary>
/// Class that contains all the information about an assembly. An assembly is made of sub-assemblies and parts
/// </summary>
class Assembly : public ObjectDB
{
protected:
	/// <summary>
	/// All the sub-assemblies of this assembly
	/// </summary>
	SubAssemblies m_subAssemblies;

	/// <summary>
	/// All the parts that this assembly is made of.
	/// </summary>
	Parts m_parts;

	/// <summary>
	/// Information container.  
	/// </summary>
	InfoAssembly m_infoAssembly;

	ModelAssembly m_modelAssembly;

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
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Rerturns a row contructed from this object data.</returns>
	Row getRow() const;

	Assemblies getSubAssemblies() const { return subAssemblies; }

	std::vector<Part> getParts() const { return parts; }

	InfoAssembly getInfoAssembly() const { return infoAssembly; }

	friend void to_json(json& j, const Assembly& m);

	friend void from_json(const json& j, Assembly& m);
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