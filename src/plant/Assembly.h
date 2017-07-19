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
	
	Position m_position; /* Position of subassembly. */
	std::shared_ptr<Assembly> m_lpAssembly; /* Pointer to assembly. */

public:

	/// <summary>
	/// Constructor.
	/// </summary>
	SubAssembly() : ObjectDB(0, "subassembly", nullptr), m_position(), m_lpAssembly(nullptr) {}

	/// <summary>
	/// Gets position of assembly.
	/// </summary>
	/// <returns></returns>
	Position getPosition() const { return m_position; }

	/// <summary>
	/// Gets assembly.
	/// </summary>
	/// <returns></returns>
	Assembly& getAssembly() const { return *m_lpAssembly; }

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
/// Definition of a vector of assemblies.
/// </summary>
typedef std::vector<Assembly> Assemblies;

/// <summary>
/// Definition of a vector of subassemblies.
/// </summary>
typedef VectorObjectDB<SubAssembly> SubAssemblies;

/// <summary>
/// Class that contains all the information about an assembly. An assembly is made of sub-assemblies and parts
/// </summary>
class Assembly : public ObjectDB
{
protected:

	SubAssemblies m_subAssemblies; /* All the sub-assemblies of this assembly */
	Parts m_parts; /* All the parts that this assembly is made of. */
	InfoAssembly m_infoAssembly; /* Information container. */
	ModelAssembly m_modelAssembly; /* Information about model. */

public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Assembly::Assembly() : ObjectDB(0, "assembly", nullptr) {}

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
	Assembly(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected = false, bool canShowInfo = false) :
		ObjectDB(id, "assembly", nullptr), m_infoAssembly(name, info, shortInfo) {}
	
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

	SubAssemblies getSubAssemblies() const { return m_subAssemblies; }

	Parts getParts() const { return m_parts; }

	InfoAssembly getInfoAssembly() const { return m_infoAssembly; }

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