#include "StockPlant.h"


void StockPlant::updateDictAssembliesFromDB(DBAdapter* lpDataBase)
{
	using namespace db;

	try
	{
		Rows rows_assemblies = lpDataBase->query("SELECT assembly.machines_id AS machine_id, machines.canshowinfo AS canshowinfo, machines.canbeselected AS canbeselected, machinetranslation.name AS name, machines.part_number AS part_number, modelsversion.path_model AS path_model, modelsversion.color AS color, modelsversion.animated AS animated, modelsversion.material AS material, machinetranslation.info AS info, machinetranslation.shortInfo AS shortinfo FROM machines INNER JOIN models ON (models.model_id = machines.Models_model_id) INNER JOIN modelsversion ON ((models.model_id = modelsversion.Models_model_id) AND (models.current_version = modelsversion.version)) INNER JOIN machinetranslation ON ((machinetranslation.Machines_machines_id = machines.machines_id) AND (machinetranslation.Language_language_id = 1))");

		for (auto it = rows_assemblies.begin(); it != rows_assemblies.end(); ++it)
		{
			int id = it->get<int>("assembly_id");

			if (m_dictAssemblies.find(id) != m_dictAssemblies.end())
			{
				m_dictAssemblies[id] = std::shared_ptr<Assembly>(new Assembly(*it));
			}
		}
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("SockPlant", e.what());
	}

}

StockPlant StockPlant::loadStockPlant(DBAdapter * lpDataBase)
{
	StockPlant plant;

	plant.setDBAdapter(lpDataBase);
	plant.loadFromDB();

	return plant;
}

std::shared_ptr<Assembly> StockPlant::getAssembly() const
{
	return m_dictAssemblies[m_assembly_id];
}

std::string StockPlant::getSN() const
{
	return m_sn;
}

bool StockPlant::isEnable() const
{
	return m_enable;
}

bool StockPlant::getCanBeSelected() const
{
	return m_canBeSelected;
}

bool StockPlant::getCanShowInfo() const
{
	return m_canShowInfo;
}

Position StockPlant::getPosition() const
{
	return m_position;
}

bool StockPlant::updateToDB(DBAdapter * lpDBAdapter)
{
	return false;
}

void StockPlant::operator=(const Row& row)
{
	this->m_assembly_id		= row.get<int>("assembly_id");
	this->m_position_id		= row.get<int>("position_id");
	this->m_sn				= row.get<std::string>("serial_number");
	this->m_canBeSelected	= row.get<bool>("canBeSelected");
	this->m_canShowInfo		= row.get<bool>("canShowInfo");
	this->m_enable			= row.get<bool>("enable");

	ObjectDB objDB(this->m_position_id, "position_entity", getDBAdapter() );
	Row rowPosition = objDB.getRowFromDB();
	this->m_position.m_pos = Vectorf3D(rowPosition.get<float>("pos_x"), rowPosition.get<float>("pos_y"), rowPosition.get<float>("pos_z"));
	this->m_position.m_rot = Vectorf3D(rowPosition.get<float>("rot_x"), rowPosition.get<float>("rot_y"), rowPosition.get<float>("rot_z"));
}

Row StockPlant::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData;

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("assembly_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("position_entity_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("serial_number", TypeData::DB_STRING));
	fieldData->push_back(FieldData("canBeSelected", TypeData::DB_BOOL));
	fieldData->push_back(FieldData("canShowInfo", TypeData::DB_BOOL));
	fieldData->push_back(FieldData("enable", TypeData::DB_BOOL));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<int>(this->m_assembly_id);
	row.addRegisterPerValue<int>(this->m_position_id);
	row.addRegisterPerValue<std::string>(this->getSN());
	row.addRegisterPerValue<bool>(this->getCanBeSelected());
	row.addRegisterPerValue<bool>(this->getCanShowInfo());
	row.addRegisterPerValue<bool>(this->isEnable());

	return row;
}

void to_json(json& j, const StockPlant& m) {
	j = json{
	{ "m_id",				m.getID() },
	{ "m_assembly_id",		m.getAssembly()->getID() },
	{ "m_position_id",		m.m_position_id },
	{ "m_position",			m.getPosition() },
	{ "m_sn",				m.getSN() },
	{ "m_canBeSelected",	m.getCanBeSelected() },
	{ "m_canShowInfo",		m.getCanShowInfo() },
	{ "m_enable",			m.isEnable() },
	{ "m_dictAssemblies",	m.m_dictAssemblies } };
}

void from_json(const json& j, StockPlant& m) {
	m.setID(j.at("m_id").get<int>());
	m.m_assembly_id		= j.at("m_assembly_id").get<int>();
	m.m_position_id		= j.at("m_position_id").get<int>();
	m.m_position		= j.at("m_position");
	m.m_sn				= j.at("m_sn").get<std::string>();
	m.m_canBeSelected	= j.at("m_canBeSelected").get<bool>();
	m.m_canShowInfo		= j.at("m_canShowInfo").get<bool>();
	m.m_enable			= j.at("m_enable").get<bool>();
	m.m_dictAssemblies	= j.at("m_dictAssemblies");
}
