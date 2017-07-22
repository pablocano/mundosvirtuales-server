#include "StockPlant.h"
#include "StockPlant.h"
#include "StockPlant.h"
#include "StockPlant.h"
#include "StockPlant.h"
#include "StockPlant.h"


StockPlant StockPlant::loadStockPlant(DBAdapter *lpDataBase)
{
	StockPlant plant;

	plant.setDBAdapter(lpDataBase);
	plant.setID(1); // ID of plant.
	plant.loadFromDB();

	return plant;
}

const Assembly& StockPlant::getAssembly() const
{
	return Assemblies::getInstance().getDictAssemblies()[m_assembly_id];
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

const SubStock& StockPlant::getSubStock() const
{
	return m_subStock;
}

bool StockPlant::loadFromDB()
{
	bool ret = ObjectDB::loadFromDB();

	if (ret)
	{
		Rows rows = getDBAdapter()->query("select child_stock_id from substock where parent_stock_id = " + std::to_string(getID()) + ";");

		for (Row row : rows)
		{
			StockPlant stock;
			stock.setDBAdapter(getDBAdapter());
			stock.setID(row.get<int>("child_stock_id"));
			stock.loadFromDB();

			m_subStock.push_back(stock);
		}
	}

	return ret;
}

bool StockPlant::updateToDB(DBAdapter * lpDBAdapter)
{
	return false;
}

void StockPlant::operator=(const Row& row)
{
	this->setID(row.get<int>("stock_id"));
	this->m_assembly_id		= row.get<int>("assembly_id");
	this->m_position_id		= row.get<int>("position_entity_id");
	this->m_sn				= row.get<std::string>("serial_number");
	this->m_canBeSelected	= row.get<bool>("canBeSelected");
	this->m_canShowInfo		= row.get<bool>("canShowInfo");
	this->m_enable			= row.get<bool>("enable");

	ObjectDB objDB(this->m_position_id, "position_entity", getDBAdapter() );
	Row rowPosition = objDB.getRowFromDB();
	this->m_position.m_pos = Vectorf3D(rowPosition.get<float>("pos_x"), rowPosition.get<float>("pos_y"), rowPosition.get<float>("pos_z"));
	this->m_position.m_rot = Vectorf3D(rowPosition.get<float>("rot_roll"), rowPosition.get<float>("rot_pitch"), rowPosition.get<float>("rot_yaw"));
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
	{ "m_assembly_id",		m.m_assembly_id },
	{ "m_position_id",		m.m_position_id },
	{ "m_position",			m.getPosition() },
	{ "m_sn",				m.getSN() },
	{ "m_canBeSelected",	m.getCanBeSelected() },
	{ "m_canShowInfo",		m.getCanShowInfo() },
	{ "m_enable",			m.isEnable() },
	{ "m_subStock",			m.getSubStock() } };
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
	m.m_subStock		= j.at("m_subStock");
}

const StockPlant& Plant::getPlant() const
{
	return m_plant;
}

const StockPlant & Plant::at(std::string sn) const
{
	return at(m_plant, sn);
}

const StockPlant& Plant::at(const StockPlant & stock, std::string sn) const
{
	if (stock.getSN() == sn)
	{
		return stock;
	}
	else
	{
		if (stock.getSubStock().size() > 0)
		{
			for (StockPlant s : stock.getSubStock())
			{
				at(s, sn);
			}
		}
	}

	static StockPlant s;
	return s;
}

void Plant::updatePlantFromDB(DBAdapter * lpDataBase)
{
	m_plant = StockPlant::loadStockPlant(lpDataBase); // TODO: optimize
}

void to_json(json & j, const Plant & m)
{
	j = json{
		{ "m_plant",	m.m_plant } };
}

void from_json(const json & j, Plant & m)
{
	m.m_plant = j.at("m_plant");
}