#include "Position.h"
#include "serialization/json.h" 

using json = nlohmann::json;

void to_json(json& j, const Vectorf3D& m)
{
	j = json{ { "x", m.x },{ "y", m.y },{ "z", m.z } };
}

void from_json(const json& j, Vectorf3D& m)
{
	m.x = j.at("x").get<float>();
	m.y = j.at("y").get<float>();
	m.z = j.at("z").get<float>();
}

void to_json(json& j, const Position& m)
{
	j = json{ { "m_pos", m.m_pos },{ "m_rot", m.m_rot } };
}

void from_json(const json& j, Position& m)
{
	m.m_pos = j.at("m_pos");
	m.m_rot = j.at("m_rot");
}