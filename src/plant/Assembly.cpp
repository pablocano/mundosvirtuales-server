#include "Assembly.h"

Assembly::Assembly()
{

}

Assembly::Assembly(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected, bool canShowInfo) :
	assembly_id(id), name(name), info(info), canShowInfo(canShowInfo), shortInfo(shortInfo), pn(pn), canBeSelected(canBeSelected)
{
}

Assembly::~Assembly()
{
}

void to_json(json& j, const Assembly& m) {
	j = json{ { "id", m.assembly_id }, {"machineParts", m.parts}, { "name", m.name },{ "info", m.info },{"canShowInfo",m.canShowInfo },{ "shortInfo", m.shortInfo },{"pn", m.pn}, {"canBeSelected", m.canBeSelected} };
}

void from_json(const json& j, Assembly& m) {
	m.assembly_id = j.at("id").get<int>();
	m.parts = j.at("machineParts");
	m.name = j.at("name").get<std::string>();
	m.info = j.at("info").get<std::string>();
	m.canShowInfo = j.at("canShowInfo").get<bool>();
	m.shortInfo = j.at("shortInfo").get<std::string>();
	m.pn = j.at("pn").get<std::string>();
	m.canBeSelected = j.at("canBeSelected").get<bool>();
}



