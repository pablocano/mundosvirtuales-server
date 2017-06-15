#pragma once

#include "../utils/serialization/json.h"
#include <vector>
#include <string>

using json = nlohmann::json;

/// <summary>
/// Contains all the information of a machine part.
/// </summary>
class MachinePart
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	MachinePart();

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="id">Unique identifier for this machine part.</param>
	/// <param name="name">The name of the machine part shown in the game.</param>
	/// <param name="material">The material of which this machine part is built of.</param>
	/// <param name="info">The description of this machine part.</param>
	/// <param name="shortInfo">A little description about the machine part.</param>
	/// <param name="pn">The part number of this machine part.</param>
	MachinePart(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn);
	
	/// <summary>
	/// Destructor
	/// </summary>
	~MachinePart();

	/// <summary>
	/// The name of this machine part.
	/// </summary>
	std::string name;

	/// <summary>
	/// The material of which this machine part is built of.
	/// </summary>
	std::string material;

	/// <summary>
	/// The descriptions of this machine part.
	/// </summary>
	std::string info;

	/// <summary>
	/// A short description of this machine part.
	/// </summary>
	std::string shortInfo;

	/// <summary>
	/// The part number of this machine part.
	/// </summary>
	std::string pn;

	/// <summary>
	/// If this machine part has a animation to shown.
	/// </summary>
	bool animated;

	/// <summary>
	/// The unique identifier of this machine part.
	/// </summary>
	int machine_id;
};

/// <summary>
/// Definition of a vector of machine parts.
/// </summary>
typedef std::vector<MachinePart> MachineParts;

/// <summary>
/// Return a json representation of the machine part class.
/// </summary>
/// <param name="j">The json structure where the to store de machine info</param>
/// <param name="m">The machine part class that is going to be stored.</param>
void to_json(json& j, const MachinePart& m);

/// <summary>
/// Creates a machine part class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The machine part where to store the information.</param>
void from_json(const json& j, MachinePart& m);