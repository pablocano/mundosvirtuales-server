#include "StockPlant.h"

#include <map>
#include <string>
#include <sstream>

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

int StockPlant::getInstance() const
{
	return m_instance;
}

void StockPlant::setInstance(int id)
{
	m_instance = id;
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

size_t StockPlant::getHash() const
{
	return m_hash;
}

void StockPlant::setHash(std::string s)
{
	std::hash<std::string> generateHash;
	m_hash = generateHash(s);
}

bool StockPlant::loadStockPerHashFromDB(size_t hash)
{
	int id = getDBAdapter()->query("SELECT stock_id FROM stock WHERE hash = " + std::to_string(hash)).front().get<int>("stock_id");
	if (id > 0)
	{
		setID(id);
		return loadFromDB();
	}
	else
	{
		return false;
	}
}

std::string StockPlant::getNodePath(std::string path)
{
	return path + (path.empty() ? "" : ",") + std::to_string(getAssemblyID()) + ":" + std::to_string(getInstance());
}

std::string StockPlant::getNodePath(std::string path, AssemblyRelation & assemblyRelation)
{
	return path + (path.empty() ? "" : ",") + std::to_string(assemblyRelation.m_id_assembly) + ":" + std::to_string(assemblyRelation.m_id_instance);
}

void StockPlant::operator=(const Row& row)
{
	this->setID(row.get<int>("stock_id"));
	this->m_assembly_id			= row.get<int>("assembly_id");
	this->m_instance			= row.get<int>("instance");
	this->m_sn					= row.get<std::string>("serial_number");
	this->m_canBeSelected		= row.get<bool>("canBeSelected");
	this->m_canShowInfo			= row.get<bool>("canShowInfo");
	this->m_enable				= row.get<bool>("enable");
	this->m_hash				= row.get<size_t>("hash");
}

Row StockPlant::getRow() const
{
	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData(this->getIDFieldName(), TypeData::DB_INTEGER, true));
	fieldData->push_back(FieldData("assembly_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("instance", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("serial_number", TypeData::DB_STRING));
	fieldData->push_back(FieldData("canBeSelected", TypeData::DB_BOOL));
	fieldData->push_back(FieldData("canShowInfo", TypeData::DB_BOOL));
	fieldData->push_back(FieldData("enable", TypeData::DB_BOOL));
	fieldData->push_back(FieldData("hash", TypeData::DB_LONG_LONG));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(this->getID());
	row.addRegisterPerValue<int>(this->m_assembly_id);
	row.addRegisterPerValue<int>(this->m_instance);
	row.addRegisterPerValue<std::string>(this->m_sn);
	row.addRegisterPerValue<bool>(this->m_canBeSelected);
	row.addRegisterPerValue<bool>(this->m_canShowInfo);
	row.addRegisterPerValue<bool>(this->m_enable);
	row.addRegisterPerValue<size_t>(this->m_hash);

	return row;
}

void to_json(json& j, const StockPlant& m) {
	j = json{
	{ "m_id",					m.getID() },
	{ "m_assembly_id",			m.m_assembly_id },
	{ "m_instance",				m.m_instance },
	{ "m_position",				m.m_position },
	{ "m_sn",					m.m_sn },
	{ "m_canBeSelected",		m.m_canBeSelected },
	{ "m_canShowInfo",			m.m_canShowInfo },
	{ "m_enable",				m.m_enable },
	{ "m_hash",					m.m_hash },
	{ "m_subStock",				m.getSubStock() } };
}

void from_json(const json& j, StockPlant& m) {
	m.setID(j.at("m_id").get<int>());
	m.m_assembly_id			= j.at("m_assembly_id").get<int>();
	m.m_instance			= j.at("m_instance").get<int>();
	m.m_position			= j.at("m_position");
	m.m_sn					= j.at("m_sn").get<std::string>();
	m.m_canBeSelected		= j.at("m_canBeSelected").get<bool>();
	m.m_canShowInfo			= j.at("m_canShowInfo").get<bool>();
	m.m_enable				= j.at("m_enable").get<bool>();
	m.m_hash				= j.at("m_hash").get<size_t>();
	m.m_subStock			= j.at("m_subStock").get<SubStock>();
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
		lpDBAdapter->onlyQuery("UPDATE plant SET value = " + std::to_string(id) + " WHERE name = 'root_stock_id';");
	}
	else
	{
		lpDBAdapter->onlyQuery("INSERT INTO plant(name, value, type_value) VALUES('root_stock_id', " + std::to_string(id) + ", 'INTEGER');");
	}

	return true;
}

void Plant::loadPlantFromDB(DBAdapter* lpDBAdapter)
{
	m_plant.setDBAdapter(lpDBAdapter);
	int idPlant = Plant::getIDRootPlant(lpDBAdapter);

	if (idPlant > 0)
	{
		m_plant.setID(idPlant);
		m_plant.loadFromDB();
	}

	// std::hash<std::string> generateHash;

	// TODO: Pablo Cano: Cargar subStock de forma recursiva
	// loadRelationFromDB(lpDBAdapter, assembly_id) retorna la lista de relaciones a partir de un id assembly
	// stock es una instancia de StockPlant
	// stock.setID(id) permite cambiar el id del objeto y stock.loadFromDB() cargarlo de la base de datos
	// loadStockPerHashFromDB(hash) carga un stock de la base de datos a partir de un hash
	// El formato del hash es una concatenacion de los nodos del recorrido del arbol, el recorrido actual se obtiene con la funcion
	// getNodePath(path) donde path es el camino del padre.
	// generateHash(path) es la funcion que devuelve el hash a partir del hash 
	// stock.setHash(s) y stock.getHash() son el setter y getter del hash
	// (PD: no pongo acentos :p para evitar problemas)
}

void Plant::updatePlantFromDB(DBAdapter* lpDBAdapter)
{
	// loadPlantFromDB(lpDBAdapter); // TODO: optimize
}

bool Plant::processRelation(DBAdapter* lpDBAdapter, AssemblyComm& assemblyComm)
{
	updatePlantFromDB(lpDBAdapter);

	for (AssemblyRelation& assemblyRelation : assemblyComm.m_listAssemblyRelations)
	{
		insertStock(lpDBAdapter, m_plant, assemblyComm.m_id_assembly, assemblyRelation);
	}

	return true;
}

bool Plant::updatePlant(DBAdapter* lpDBAdapter, AssemblyComm& assemblyComm)
{
	return false;
}

void Plant::insertStock(DBAdapter* lpDBAdapter, StockPlant& root, int idAssembly, AssemblyRelation& assemblyRelation)
{
	std::string path;
	if (!m_plant.isValidID())
	{
		m_plant.setDBAdapter(lpDBAdapter);
		m_plant.setAssemblyID(idAssembly);
		m_plant.setHash(StockPlant::getNodePath(path, assemblyRelation));
		m_plant.saveToDB();
	}
	//else if (root.getAssemblyID() == assemblyRelation.m_id_assembly && m_plant.getID() == root.getID())
	//{
	//	if(root.isValidID())
	//		stock.m_subStock.push_back(root);

	//	// Update root plant
	//	m_plant = stock;
	//	setIDRootPlant(lpDBAdapter, stock.getID());

	//	// Update Hash
	//	path.push_back(stock.getAssemblyID());
	//	changeHash(lpDBAdapter, m_plant, path);

	//	return root.getID();
	//}
	else
	{
		insertStock(lpDBAdapter, root, idAssembly, assemblyRelation, path);
	}
}

void Plant::insertStock(DBAdapter* lpDBAdapter, StockPlant& root, int idAssembly, AssemblyRelation& assemblyRelation, std::string path)
{
	if (root.m_subStock.size() > 0)
	{
		std::string pathRoot = root.getNodePath(path);

		for (auto it = root.m_subStock.begin(); it != root.m_subStock.end() ; ++it)
		{
			if (it->getAssemblyID() == assemblyRelation.m_id_assembly)
			{
				// Create New Stock
				StockPlant stock;
				stock.setDBAdapter(lpDBAdapter);
				stock.setAssemblyID(idAssembly);
				stock.setHash(StockPlant::getNodePath(path, assemblyRelation));
				stock.saveToDB();

				// Insert new stock
				StockPlant s = *it;
				stock.m_subStock.push_back(s);
				root.m_subStock.erase(it);

				// Update Hash
				changeHash(lpDBAdapter, stock.m_subStock.back(), pathRoot);
			}
			else
			{
				insertStock(lpDBAdapter, *it, idAssembly, assemblyRelation, pathRoot);
			}
		}
	}
}

void Plant::changeHash(DBAdapter* lpDBAdapter, StockPlant& root, std::string path)
{
	root.setHash(root.getNodePath(path));
	root.saveToDB();

	for (StockPlant s : root.getSubStock())
	{
		changeHash(lpDBAdapter, s, root.getNodePath(path));
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