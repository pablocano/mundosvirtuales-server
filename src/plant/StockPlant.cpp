#include "StockPlant.h"

Assembly StockPlant::getAssembly() const
{
	return m_assembly;
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

bool StockPlant::loadFromDB(DBAdapter* lpDBAdapter, int id)
{
	return false;
}

bool StockPlant::saveToDB(DBAdapter* lpDBAdapter)
{
	return false;
}

void StockPlant::setFromRow(DBAdapter* lpDBAdapter, const Row& row)
{
	m_assembly.loadFromDB(lpDBAdapter, row.get<int>("assembly_id"));
	m_position.loadFromDB(lpDBAdapter, row.get<int>("position_id"));
	m_sn				= row.get<std::string>("serial_number");
	m_canBeSelected		= row.get<bool>("canBeSelected");
	m_canShowInfo		= row.get<bool>("canShowInfo");
	m_enable			= row.get<bool>("enable");
}

Row StockPlant::getRow()
{
	return Row();
}

void to_json(json& j, const StockPlant& m) {
	j = json{ 
	{ "m_assembly",			m.m_assembly },
	{ "m_position",			m.info },
	{ "m_sn",				m.shortInfo },
	{ "m_canBeSelected",	m.m_canBeSelected },
	{ "m_canShowInfo",		m.m_canShowInfo },
	{ "m_enable",			m.m_enable } };
}

void from_json(const json& j, StockPlant& m) {
	m.m_assembly		= j.at("m_assembly");
	m.m_position		= j.at("m_position");
	m.m_sn				= j.at("m_sn").get<std::string>();
	m.m_canBeSelected	= j.at("m_canBeSelected").get<bool>();
	m.m_canShowInfo		= j.at("m_canShowInfo").get<bool>();
	m.m_enable			= j.at("m_enable").get<bool>();
}