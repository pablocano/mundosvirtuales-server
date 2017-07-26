#pragma once

#include "../utils/serialization/json.h"
#include "../utils/db/ObjectDB.h"

using json = nlohmann::json;

class InfoAssembly : public ObjectDB
{
protected:

	std::string m_name; /* The name of the machine. */
	std::string m_info; /* The descriptions of this assembly. */
	std::string m_shortInfo; /* A short description of this assembly. */
	int m_language_id; /* Id of default language. */
	int m_assembly_id; /* Id of assembly. */

public:
	/// <summary>
	/// Constructor.
	/// </summary>
	InfoAssembly() : ObjectDB(0, "assembly_translation", nullptr), m_name("NO NAME"), m_info(), m_shortInfo(), m_language_id(1), m_assembly_id() {}

	/// <summary>
	/// Gets Name.
	/// </summary>
	std::string getName() const;

	/// <summary>
	/// Gets information about this assembly.
	/// </summary>
	std::string getInfo() const;

	/// <summary>
	/// Gets short information about this assembly.
	/// </summary>
	std::string getShortInfo() const;

	/// <summary>
	/// Gets Assembly ID.
	/// </summary>
	/// <returns>Returns id of assembly. </returns>
	int InfoAssembly::getAssemblyID() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	void setAssemblyID(int id);

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

	friend void to_json(json& j, const InfoAssembly& m);

	friend void from_json(const json& j, InfoAssembly& m);
};

void to_json(json& j, const InfoAssembly& m);

void from_json(const json& j, InfoAssembly& m);
