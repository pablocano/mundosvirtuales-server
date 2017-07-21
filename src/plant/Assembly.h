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

	std::string getPN() const;

	InfoAssembly getInfo() const;

	ModelAssembly getModel() const;

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

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getWhere() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getJoin() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getFieldsSelect() const;

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

/// <summary>
/// Dict Assemblies.
/// </summary>
typedef std::map<int, Assembly> DictAssemblies;

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

	Assembly& operator[](int id)
	{
		return m_dictAssemblies[id];
	}

	void updateDictAssembliesFromDB(DBAdapter* lpDataBase);
	
	friend void to_json(json& j, const Assemblies& m);

	friend void from_json(const json& j, Assemblies& m);
};


void to_json(json& j, const Assemblies& m);

void from_json(const json& j, Assemblies& m);