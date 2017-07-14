#pragma once

#include "../utils/serialization/json.h"
#include "../utils/db/ObjectDB.h"

using json = nlohmann::json;

class InfoAssembly : public ObjectDB
{
protected:
	/// <summary>
	/// The name of the machine
	/// </summary>
	std::string m_name;

	/// <summary>
	/// The descriptions of this assembly.
	/// </summary>
	std::string m_info;

	/// <summary>
	/// A short description of this assembly.
	/// </summary>
	std::string m_shortInfo;

public:
	/// <summary>
	/// Constructor.
	/// </summary>
	InfoAssembly() : ObjectDB(0, "assembly_translation"), m_name("NO NAME"), m_info(), m_shortInfo() {}

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="name">The name of the machine shown in the game.</param>
	/// <param name="info">The description of this assembly.</param>
	/// <param name="shortInfo">A little description about the assembly.</param>
	InfoAssembly(std::string name, std::string info, std::string shortInfo) : ObjectDB(0, "assembly_translation"), m_name(name), m_info(info), m_shortInfo(shortInfo) {}

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
