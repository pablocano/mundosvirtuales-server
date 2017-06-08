#pragma once

#include "../utils/serialization\json.h"
#include "MachinePart.h"
#include <vector>
#include <string>

using json = nlohmann::json;

class Machine
{
public:
	Machine();

	Machine(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected = false, bool canShowInfo = false);
	
	~Machine();

	std::vector<MachinePart> machineParts;

	std::string name;

	bool canShowInfo;

	std::string info;

	std::string shortInfo;

	std::string pn;

	bool canBeSelected;

	int machine_id;
};

typedef std::vector<Machine> Machines;

void to_json(json& j, const Machine& m);

void from_json(const json& j, Machine& m);