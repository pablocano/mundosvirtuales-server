#pragma once

#include "../utils/serialization/json.h"
#include <vector>
#include <string>

using json = nlohmann::json;

/// <summary>
/// Contains all the information of a part.
/// </summary>
class Part
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