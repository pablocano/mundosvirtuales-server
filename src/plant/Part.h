#pragma once

#include "../plant/ObjectPlant.h"
#include "../utils/serialization/json.h" 
#include <vector>
#include <string>

using json = nlohmann::json;
using namespace db;

/// <summary>
/// Contains all the information of a part.
/// </summary>
class Part : public ObjectPlant
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Part();

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="id">Unique identifier for this part.</param>
	/// <param name="name">The name of the machine part shown in the game.</param>
	/// <param name="material">The material of which this part is built of.</param>
	/// <param name="info">The description of this part.</param>
	/// <param name="shortInfo">A little description about the part.</param>
	/// <param name="pn">The part number of this part.</param>
	Part(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn);
	
	/// /// <summary>
	/// Constructor object from row.
	/// </summary>
	/// <param name="row">Row</param>
	Part(const Row& row) : Part() { *this = row; }

	/// <summary>
	/// Load properties this object from database.
	/// </summary>
	/// <param name="lpDBAdapter">Pointer to the database handle.</param>
	/// <param name="id">Identifier of Assembly in the database.</param>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	bool loadFromDB(DBAdapter* lpDBAdapter, int id);

	/// <summary>
	/// Save part to database.
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
	/// Destructor
	/// </summary>
	~Part();

	/// <summary>
	/// The name of this part.
	/// </summary>
	std::string name;

	/// <summary>
	/// The material of which this part is built of.
	/// </summary>
	std::string material;

	/// <summary>
	/// The descriptions of this part.
	/// </summary>
	std::string info;

	/// <summary>
	/// A short description of this part.
	/// </summary>
	std::string shortInfo;

	/// <summary>
	/// The part number of this part.
	/// </summary>
	std::string pn;

	/// <summary>
	/// If this part has a animation to shown.
	/// </summary>
	bool animated;

	/// <summary>
	/// The unique identifier of this part.
	/// </summary>
	int part_id;
};

/// <summary>
/// Definition of a vector of parts.
/// </summary>
typedef std::vector<Part> Parts;

/// <summary>
/// Return a json representation of the part class.
/// </summary>
/// <param name="j">The json structure where the to store de part info</param>
/// <param name="m">The part class that is going to be stored.</param>
void to_json(json& j, const Part& m);

/// <summary>
/// Creates a part class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The part where to store the information.</param>
void from_json(const json& j, Part& m);