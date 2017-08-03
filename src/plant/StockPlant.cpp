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

void StockPlant::deleteSubStock()
{
	for (StockPlant& s : m_subStock)
	{
		s.deleteSubStock();
	}

	deleteToDB();
	m_subStock.clear();
}

bool StockPlant::loadStockPerHashFromDB(size_t hash)
{
	Rows rows = getDBAdapter()->query("SELECT stock_id FROM stock WHERE hash = " + std::to_string(hash));
	if (!rows.isEmpty())
	{
		int id = rows.front().get<int>("stock_id");
		if (id > 0)
		{
			setID(id);
			return loadFromDB();
		}
	}
	
	return false;
}

std::string StockPlant::getNodePath(std::string path)
{
	return path + (path.empty() ? "" : ",") + std::to_string(getAssemblyID()) + ":" + std::to_string(getInstance());
}

void StockPlant::createStock(DBAdapter* lpDBAdapter, AssemblyRelation& assemblyRelation, const std::string& path)
{
	// Initialize the sub stock adapter
	setDBAdapter(lpDBAdapter);

	// Set the id of the stock
	setAssemblyID(assemblyRelation.m_child_assembly_id);

	// Set the instance of the stock
	setInstance(assemblyRelation.m_instance);

	// Set the hash of the stock
	setHash(getNodePath(path));

	// Save the current stock into the database
	saveToDB();

	// Obtain all the relations of the assembly of the current stock
	ListAssemblyRelations relations = Assemblies::loadRelationFromDB(getDBAdapter(), m_assembly_id);

	// Iterate over all the relations
	for (auto& relation : relations)
	{
		// Create the substock
		StockPlant childStock;

		// Add the substock into the tree
		childStock.createStock(lpDBAdapter, relation, getNodePath(path));

		// Add the substock into the substocks
		m_subStock.push_back(childStock);
	}
}

std::string StockPlant::getNodePath(std::string path, AssemblyRelation & assemblyRelation)
{
	return path + (path.empty() ? "" : ",") + std::to_string(assemblyRelation.m_child_assembly_id) + ":" + std::to_string(assemblyRelation.m_instance);
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
	m.m_position			= j.at("m_position").get<Position>();
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
	//Set the DB adapter of the root
	m_plant.setDBAdapter(lpDBAdapter);

	// Get the id off the root
	int idPlant = Plant::getIDRootPlant(lpDBAdapter);

	// If the id is valid
	if (idPlant > 0)
	{
		// Set the id to the root stock
		m_plant.setID(idPlant);
		
		// Load the root from DB
		m_plant.loadFromDB();
	}

	//Load all substocks recursively
	m_plant.AddSubStocks(m_plant.getNodePath(""));
}

void StockPlant::AddSubStocks(const std::string & path)
{
	// Creation of the function that generates the hash
	std::hash<std::string> generateHash;

	// Obtain all the relations of the assembly of the current stock
	ListAssemblyRelations relations = Assemblies::loadRelationFromDB(getDBAdapter(), m_assembly_id);

	// Iterate over all relations the assembly of this stock
	for (const auto& relation : relations)
	{
		// Creation the possible child stock
		StockPlant childStock;

		// Set handler Database.
		childStock.setDBAdapter(getDBAdapter());

		// Load the stock from the DB by the path of the relation
		if (childStock.loadStockPerHashFromDB(generateHash(relation.CreatePath(path))))
		{
			// If the stock is load correctly, load its substocks and then add it to the substocks
			if (childStock.m_assembly_id > 0 && childStock.m_assembly_id == relation.m_child_assembly_id)
			{
				// Load its substocks
				childStock.AddSubStocks(getNodePath(path));

				// Add it to the substock list of the current stock
				m_subStock.push_back(childStock);
			}
		}
	}
}


void StockPlant::UpdateStock(const std::string & path, int caller_assembly_id)
{
	// Verify if the current assembly must be updated
	if (m_assembly_id != caller_assembly_id)
	{
		// Call the update function for each substock
		for (auto& substock : m_subStock)
		{
			substock.UpdateStock(getNodePath(path), caller_assembly_id);
		}
	}
	else
	{
		// Obtain all the relations of the assembly of the current stock
		ListAssemblyRelations relations = Assemblies::loadRelationFromDB(getDBAdapter(), m_assembly_id);

		// Iterate over all the substocks
		for (auto& substock = m_subStock.begin(); substock != m_subStock.end();)
		{
			// Delete every substock that is not present in the relations
			bool deleteSubStock = true;

			// Iterate over the relations
			for (auto& relation : relations)
			{
				// If the relation is the same as the substock, update it
				if (substock->m_assembly_id == relation.m_child_assembly_id && substock->m_instance == relation.m_instance)
				{
					// Update substock
					substock->UpdateStock(getNodePath(path), caller_assembly_id);

					// Mark the relation as used
					relation.m_child_assembly_id = -1;

					// Do not delete this substock
					deleteSubStock = false;

					// Stop searching in the relations
					break;
				}
			}

			// If this substock is not present in the relations, delete it
			if (deleteSubStock)
			{
				// Remove the substock from the tree
				substock->deleteSubStock();

				// Remove the sub stock from the sub stocks and update the iteator to the next substock
				substock = m_subStock.erase(substock);
			}
			else
			{
				// Update the iterator to the next substock
				substock++;
			}
		}

		// Create a new substock for each not used relation
		for (auto& relation : relations)
		{
			// Verify that the relation was not used before
			if (relation.m_child_assembly_id > 0)
			{
				// Creation the sub stock
				StockPlant childStock;

				// Add the substock into the tree
				childStock.createStock(getDBAdapter(), relation, getNodePath(path));

				// Add the child as a substock
				m_subStock.push_back(childStock);
			}
		}
	}

}

void Plant::UpdateTree(DBAdapter* lpDBAdapter, const AssemblyComm & assemblyComm)
{
	// If the root in not valid or if the new assembly is the new root, set as root
	bool f = Assemblies::IsConnected(lpDBAdapter, assemblyComm.m_id_assembly, m_plant.m_assembly_id);
	if (!m_plant.isValidID() || f)
	{
		// Set the new assembly as root
		m_plant.setAssemblyID(assemblyComm.m_id_assembly);

		// Set the adapter to communicate with the database
		m_plant.setDBAdapter(lpDBAdapter);

		// Set the new hash of the root
		m_plant.setHash(m_plant.getNodePath(""));

		// Save the root into the database
		m_plant.saveToDB();
		Plant::setIDRootPlant(lpDBAdapter, m_plant.getID());
	}

	// Update the stock tree using the new assembly
	m_plant.UpdateStock(m_plant.getNodePath(""), assemblyComm.m_id_assembly);

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