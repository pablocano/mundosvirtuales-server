#include "Machine.h"

Machine::Machine()
{
}

Machine::Machine(int id, std::string name, std::string info, std::string shortInfo, std::string pn, bool canBeSelected, bool canShowInfo) :
	machine_id(id), name(name), info(info), canShowInfo(canShowInfo), shortInfo(shortInfo), pn(pn), canBeSelected(canBeSelected)
{
}

Machine::~Machine()
{
}

void Machine::updateState()
{
}

std::vector<float> Machine::getOutput(float dt)
{
	return m_states;
}

void to_json(json& j, const Machine& m) {
	j = json{ { "id", m.machine_id }, {"machineParts", m.machineParts}, { "name", m.name },{ "info", m.info },{"canShowInfo",m.canShowInfo },{ "shortInfo", m.shortInfo },{"pn", m.pn}, {"canBeSelected", m.canBeSelected} };
}

void from_json(const json& j, Machine& m) {
	m.machine_id = j.at("id").get<int>();
	m.machineParts = j.at("machineParts");
	m.name = j.at("name").get<std::string>();
	m.info = j.at("info").get<std::string>();
	m.canShowInfo = j.at("canShowInfo").get<bool>();
	m.shortInfo = j.at("shortInfo").get<std::string>();
	m.pn = j.at("pn").get<std::string>();
	m.canBeSelected = j.at("canBeSelected").get<bool>();
}
