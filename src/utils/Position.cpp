#include "Position.h"
#include "serialization/json.h" 

using json = nlohmann::json;

Position loadPositionFromDB(DBAdapter* lpDBAdapter, int position_id)
{
	ObjectDB objDB(position_id, "position_entity", lpDBAdapter);
	Row rowPosition = objDB.getRowFromDB();
	Position position;
	position.m_pos = Vectorf3D(rowPosition.get<float>("pos_x"), rowPosition.get<float>("pos_y"), rowPosition.get<float>("pos_z"));
	position.m_rot = Vectorf3D(rowPosition.get<float>("rot_roll"), rowPosition.get<float>("rot_pitch"), rowPosition.get<float>("rot_yaw"));

	return position;
}

int savePositionToDB(DBAdapter* lpDBAdapter, const Position & position)
{
	ObjectDB objDB(0, "position_entity", lpDBAdapter);
	Row rowPosition = objDB.getRowFromDB();

	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(objDB.getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("pos_x", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("pos_y", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("pos_z", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_roll", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_pitch", TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_yaw", TypeData::DB_DOUBLE));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(objDB.getID());
	row.addRegisterPerValue<double>(position.m_pos.x);
	row.addRegisterPerValue<double>(position.m_pos.y);
	row.addRegisterPerValue<double>(position.m_pos.z);
	row.addRegisterPerValue<double>(position.m_rot.x);
	row.addRegisterPerValue<double>(position.m_rot.y);
	row.addRegisterPerValue<double>(position.m_rot.z);

	if (objDB.saveToDB(row))
		return objDB.getID();
	else
		return -1;
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
	j = json{ { "m_pos", m.m_pos },{ "m_rot", m.m_rot } };
}

void from_json(const json& j, Position& m)
{
	m.m_pos = j.at("m_pos");
	m.m_rot = j.at("m_rot");
}