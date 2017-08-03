#include "Position.h"
#include "serialization/json.h" 

using json = nlohmann::json;

Row Position::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("pos_x", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("pos_y", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("pos_z", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_roll", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_pitch", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_yaw", TypeData::DB_DOUBLE));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(getID());
	row.addRegisterPerValue<double>(this->m_pos.x);
	row.addRegisterPerValue<double>(this->m_pos.y);
	row.addRegisterPerValue<double>(this->m_pos.z);
	row.addRegisterPerValue<double>(this->m_rot.x);
	row.addRegisterPerValue<double>(this->m_rot.y);
	row.addRegisterPerValue<double>(this->m_rot.z);

	return row;
}

void Position::operator=(const Row& row)
{
	ObjectDB::operator=(row);
	this->m_pos = Vectorf3D(row.get<float>("pos_x"), row.get<float>("pos_y"), row.get<float>("pos_z"));
	this->m_rot = Vectorf3D(row.get<float>("rot_roll"), row.get<float>("rot_pitch"), row.get<float>("rot_yaw"));
}

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
	j = json{
		{ "m_pos",	m.m_pos },
		{ "m_rot",	m.m_rot } };
}

void from_json(const json& j, Position& m)
{
	m.m_pos = j.at("m_pos");
	m.m_rot = j.at("m_rot");
}