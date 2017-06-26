#pragma once

#include "../utils/serialization/json.h"
#include "../utils/Position.h"
#include "ModelControl.h"
#include "MachinePart.h"
#include <vector>
#include <string>

using json = nlohmann::json;

/// <summary>
/// Class that contains all the information about a machine.
/// </summary>
class Machine : public ModelControl
{
public:
	/// <summary>
	/// Default constructor
	/// </summary>
	Machine();

	/// <summary>
	/// Constructor
	/// </summary>
	/// <param name="id">Unique identifier for this machine.</param>
	/// <param name="name">The name of the machine shown in the game.</param>
	/// <param name="info">The description of this machine.</param>
	/// <param name="shortInfo">A little description about the machine.</param>
	/// <param name="pn">The part number of this machine.</param>
	/// <param name="canBeSelected">If this part can be selected in the virtual environment.</param>
	/// <param name="canShowInfo">If this past has some info to shown.</param>
	Machine(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected = false, bool canShowInfo = false);
	
	/// <summary>
	/// Destructor
	/// </summary>
	~Machine();

	/// <summary>
	/// Update state machine.
	/// </summary>
	void updateState();

	/// <summary>
	/// Gets output model machine.
	/// </summary>
	/// <param name="dt">Delta time</param>
	/// <returns>Returns model states.</returns>
	std::vector<float> getOutput(float dt);

	/// <summary>
	/// All the part that this machine is made of.
	/// </summary>
	std::vector<MachinePart> machineParts;

	/// <summary>
	/// The name of the machine
	/// </summary>
	std::string name;

	/// <summary>
	/// If this machine has inforation to shown.
	/// </summary>
	bool canShowInfo;

	/// <summary>
	/// The descriptions of this machine.
	/// </summary>
	std::string info;

	/// <summary>
	/// A short description of this machine.
	/// </summary>
	std::string shortInfo;

	/// <summary>
	/// The part number of this machine.
	/// </summary>
	std::string pn;

	/// <summary>
	/// If this machine can be selected in the virtual environment.
	/// </summary>
	bool canBeSelected;

	/// <summary>
	/// The unique identifier of this machine.
	/// </summary>
	int machine_id;

	/// <summary>
	/// Position machine.
	/// </summary>
	Position position;
};

/// <summary>
/// Definition of a vector of machines.
/// </summary>
typedef std::vector<Machine> Machines;

/// <summary>
/// Return a json representation of the machine class.
/// </summary>
/// <param name="j">The json structure where the to store de machine info</param>
/// <param name="m">The machine class that is going to be stored.</param>
void to_json(json& j, const Machine& m);

/// <summary>
/// Creates a machine class from a json info.
/// </summary>
/// <param name="j">The json structure where the information is.</param>
/// <param name="m">The machine where to store the information.</param>
void from_json(const json& j, Machine& m);