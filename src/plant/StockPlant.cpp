#include "StockPlant.h"

#include <map>


StockPlant StockPlant::loadStockPlant(DBAdapter *lpDBAdapter)
{
	StockPlant plant;

	plant.setDBAdapter(lpDBAdapter);
	int idPlant = Plant::getIDRootPlant(lpDBAdapter);
	
	if (idPlant > 0)
	{
		plant.setID(idPlant);
		plant.loadFromDB();
	}

	return plant;
}

const Assembly& StockPlant::getAssembly() const
{
	return Assemblies::getInstance().getDictAssemblies()[m_assembly_id];
}

int StockPlant::getAssemblyID() const
{
	return m_assembly_id;
}

void StockPlant::setAssemblyID(int id)
{
	m_assembly_id = id;
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

void StockPlant::setPosition(const Position & position)
{
	m_position = position;
}

const SubStock& StockPlant::getSubStock() const
{
	return m_subStock;
}

void StockPlant::addStock(StockPlant & stock)
{
	m_subStock.push_back(stock);
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

bool StockPlant::saveToDB()
{
	if (m_position_id <= 0)
		m_position_id = savePositionToDB(m_position, getDBAdapter());

	if (m_position_id)
		return ObjectDB::saveToDB();
	else
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

	this->m_position = loadPositionFromDB(this->m_position_id, getDBAdapter());
}

Position StockPlant::loadPositionFromDB(int position_id, DBAdapter* lpDBAdapter)
{
	ObjectDB objDB(position_id, "position_entity", lpDBAdapter);
	Row rowPosition = objDB.getRowFromDB();
	Position position;
	position.m_pos = Vectorf3D(rowPosition.get<float>("pos_x"), rowPosition.get<float>("pos_y"), rowPosition.get<float>("pos_z"));
	position.m_rot = Vectorf3D(rowPosition.get<float>("rot_roll"), rowPosition.get<float>("rot_pitch"), rowPosition.get<float>("rot_yaw"));

	return position;
}

int StockPlant::savePositionToDB(const Position& position, DBAdapter* lpDBAdapter)
{
	ObjectDB objDB(0, "position_entity", lpDBAdapter);
	Row rowPosition = objDB.getRowFromDB();
	
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(objDB.getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("pos_x",		TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("pos_y",		TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("pos_z",		TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_roll",	TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_pitch",	TypeData::DB_DOUBLE));
	fieldData->push_back(FieldData("rot_yaw",	TypeData::DB_DOUBLE));

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

Row StockPlant::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER, true));
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

const StockPlant& Plant::at(int id) const
{
	return at(m_plant, id);
}

const StockPlant& Plant::at(const StockPlant & stock, int id) const
{
	if (stock.getID() == id)
	{
		return stock;
	}
	else
	{
		if (stock.getSubStock().size() > 0)
		{
			for (StockPlant s : stock.getSubStock())
			{
				at(s, id);
			}
		}
	}

	static StockPlant s; // return default stock
	return s;
}

const StockPlant& Plant::at(std::string sn) const
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

	static StockPlant s; // return default stock
	return s;
}

int Plant::getIDRootPlant(DBAdapter * lpDBAdapter)
{
	Rows rows = lpDBAdapter->query("SELECT * FROM plant;");
	for (Row& row : rows)
	{
		if (row.get<std::string>("name") == "root_stock_id")
		{
			return std::stoi(row.get<std::string>("value"));
		}
	}
	return -1;
}

bool Plant::setIDRootPlant(DBAdapter * lpDBAdapter, int id)
{
	if (lpDBAdapter->countQuery("plant", "name = 'root_stock_id'") > 0)
	{
		lpDBAdapter->query("INSERT INTO plant(name, value, value_type) VALUES('root_stock_id', " + std::to_string(id) + ", 'INTEGER');");
	}
	else
	{
		lpDBAdapter->query("UPDATE plant SET value = " + std::to_string(id) + " WHERE name = 'root_stock_id';");
	}

	return true;
}

void Plant::loadPlantFromDB(DBAdapter* lpDBAdapter)
{
	m_plant = StockPlant::loadStockPlant(lpDBAdapter);
}

void Plant::updatePlantFromDB(DBAdapter* lpDBAdapter)
{
	// loadPlantFromDB(lpDBAdapter); // TODO: optimize
}

StockPlant Plant::newStock(DBAdapter * lpDBAdapter, int idAssembly)
{
	StockPlant stock;
	stock.setDBAdapter(lpDBAdapter);
	stock.setAssemblyID(idAssembly);

	stock.saveToDB();

	return stock;
}

bool Plant::processRelation(DBAdapter * lpDBAdapter, int idAssembly, AssemblyComm& assemblyComm)
{
	//static std::map<int, std::vector<StockPlant>> dictStock;

	//updatePlantFromDB(lpDBAdapter);

	//StockPlant& rootStock = newStock(lpDBAdapter, idAssembly, assemblyComm);

	//if (!m_plant.isValidID())
	//{
	//	m_plant = rootStock;
	//	setIDRootPlant(lpDBAdapter, rootStock.getID());
	//}

	//for (AssemblyRelation& assemblyRelation : assemblyComm.m_listAssemblyRelations)
	//{
	//	StockPlant& stock = newStock(lpDBAdapter, assemblyComm.m_id_assembly);
	//	
	//	if (!m_plant.isValidID())
	//	{
	//		m_plant = stock;
	//		setIDRootPlant(lpDBAdapter, stock.getID());
	//	}
	//	else
	//	{

	//	}
	//	
	//	insertStock(stock)
	//	if (dictStock.find(assemblyRelation.m_id_instance) == dictStock.end())
	//	{
	//		// Doesn't exist assembly
	//		StockPlant stock;
	//		stock.setDBAdapter(lpDBAdapter);
	//		stock.setPosition(assemblyRelation.m_position);
	//		if (Assemblies::getInstance().existAssembly(idAssembly))
	//		{
	//			stock.setAssemblyID(idAssembly);
	//		}
	//		
	//		if (stock.saveToDB())
	//		{
	//			stockInstances[std::to_string(assemblyRelation.m_id_instance)] = stock;


	//		}

	//	}

	//	if (assemblyRelation.m_id_assembly > 0)
	//	{
	//		// TODO: falta codigo aca
	//		StockPlant stock;
	//		stock.setDBAdapter(lpDBAdapter);
	//		stock.setPosition(assemblyRelation.m_position);
	//		stock.setAssemblyID(idAssembly);			
	//	}
	//}

	return true;
}

bool Plant::insertStock(StockPlant& root, StockPlant& stock, int parent_assembly_id)
{
	if (root.getAssemblyID() == parent_assembly_id)
	{
		root.addStock(stock);
		return true;
	}
	else
	{
		if (root.getSubStock().size() > 0)
		{
			for (StockPlant s : root.getSubStock())
			{
				insertStock(s, stock, parent_assembly_id);
			}
		}
		else
			return false;
	}
}

void Plant::setPlant(json j)
{
	from_json(j, *this);
}

void to_json(json & j, const Plant & m)
{
	j = json{ { "m_plant",	m.m_plant } };
}

void from_json(const json & j, Plant & m)
{
	m.m_plant = j.at("m_plant");
}