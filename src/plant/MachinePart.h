#pragma once

#include "../utils/serialization/json.h"
#include <vector>
#include <string>

using json = nlohmann::json;

class MachinePart
{
public:
	MachinePart();

	MachinePart(int id, std::string name, std::string material, std::string info, std::string shortInfo, std::string pn);
	
	~MachinePart();

	std::string name;

	std::string material;

	std::string info;

	std::string shortInfo;

	std::string pn;

	bool animated;

	int machine_id;
};

typedef std::vector<MachinePart> MachineParts;

void to_json(json& j, const MachinePart& m);

void from_json(const json& j, MachinePart& m);