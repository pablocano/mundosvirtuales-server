#pragma once

#include "../plant/sensors/Sensor.h"
#include "../utils/db/ObjectDB.h"
#include "../utils/serialization/json.h"
#include "../utils/Position.h"
#include "Assembly.h"

using json = nlohmann::json;
using namespace db;

class StockPlant;

class Plant;

/// <summary>
/// Definition of a vector of stock.
/// </summary>
typedef std::vector<StockPlant> SubStock;

/// <summary>
/// Definition of a vector of assembly identifies.
/// </summary>
typedef std::vector<int> IDStock;


class StockPlant : public ObjectDB
{
public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	/// <returns></returns>
	static StockPlant loadStockPlant(DBAdapter* lpDBAdapter);

protected:
	
	int m_assembly_id; /* Assembly Identifier. */
	int m_position_id; /* Position Identifier. */
	Position m_position; /* Position on plant. */
	std::string m_sn; /* Serial number. */
	bool m_canBeSelected; /* This flag is used to know if the stock can be selected. */
	bool m_canShowInfo; /* This flag is used to know if the stock can be showed.  */
	bool m_enable; /* This flag is used to know if the stock is enabled. */
	size_t m_hash; /* Hash for identified stock (creation). */
	int m_assembly_instance; /* Disambiguation for each assembly. */
	SubStock m_subStock; /* All the sub-assemblies of this assembly */

public:
	/// <summary>
	/// Constructor.
	/// </summary>
	StockPlant() : ObjectDB(0, "stock", nullptr), m_assembly_id(), m_position_id(), m_position(), m_sn("SN"), m_canBeSelected(false), m_canShowInfo(false), m_enable(false), m_hash(), m_assembly_instance() {}

	/// <summary>
	/// Gets Assembly.
	/// </summary>
	const Assembly& getAssembly() const;

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int getAssemblyID() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	void setAssemblyID(int id);

	/// <summary>
	/// Gets Serial Number.
	/// </summary>
	std::string getSN() const;

	/// <summary>
	/// Gets true if this object is enabled, false otherwise.
	/// </summary>
	bool isEnable() const;

	/// <summary>
	/// Gets true if this object can be selected.
	/// </summary>
	bool getCanBeSelected() const;

	/// <summary>
	/// Gets true if this object can show informartion.
	/// </summary>
	bool getCanShowInfo() const;

	/// <summary>
	/// Gets position.
	/// </summary>
	Position getPosition() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	void setPosition(const Position& position);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const SubStock& getSubStock() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="stock"></param>
	void addStock(StockPlant& stock);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="stock"></param>
	void delStock(StockPlant& stock);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	size_t getHash() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="ids"></param>
	void generateHash(std::vector<int> ids);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int getAssemblyInstance() const;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="instance"></param>
	void setAssemblyInstance(int instance);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool loadFromDB();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool saveToDB();

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	void operator=(const Row& row);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position_id"></param>
	/// <param name="lpDBAdapter"></param>
	/// <returns></returns>
	static Position loadPositionFromDB(int position_id, DBAdapter* lpDBAdapter);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	/// <param name="lpDBAdapter"></param>
	/// <returns></returns>
	int savePositionToDB(const Position& position, DBAdapter* lpDBAdapter);

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Rerturns a row contructed from this object data.</returns>
	Row getRow() const;

	friend void to_json(json& j, const StockPlant& m);

	friend void from_json(const json& j, StockPlant& m);

	friend class Plant;
};


void to_json(json& j, const StockPlant& m);

void from_json(const json& j, StockPlant& m);

class Plant
{
private:
	StockPlant m_plant;

public:
	static Plant& getInstance()
	{
		static Plant instance;
		return instance;
	}
private:
	Plant() : m_plant() {}

public:
	Plant(Plant const&) = delete;
	void operator=(Plant const&) = delete;

	const StockPlant& getPlant() const;

	const StockPlant& at(int id) const;

	const StockPlant& at(const StockPlant & stock, int id) const;

	const StockPlant& at(std::string sn) const;

	const StockPlant& at(const StockPlant& stock, std::string sn) const;

	static int getIDRootPlant(DBAdapter* lpDBAdapter);

	static bool setIDRootPlant(DBAdapter* lpDBAdapter, int id);

	void loadPlantFromDB(DBAdapter* lpDBAdapter);

	void updatePlantFromDB(DBAdapter* lpDBAdapter);

	StockPlant newStock(DBAdapter* lpDBAdapter, int idAssembly, int instance_id, const Position& position);
	
	StockPlant newStock(DBAdapter* lpDBAdapter, int idAssembly, int instance_id);

	bool processRelation(DBAdapter* lpDBAdapter, int idAssembly, AssemblyComm& assemblyComm);

	bool updateRelation(DBAdapter* lpDBAdapter, int idAssembly, AssemblyComm& assemblyComm);

	bool updateRelation(DBAdapter* lpDBAdapter, int idAssembly, AssemblyComm& assemblyComm, std::vector<int> path);

	int insertStock(DBAdapter* lpDBAdapter, StockPlant& root, StockPlant& stock, int child_assembly_id);
	
	int insertStock(DBAdapter* lpDBAdapter, StockPlant& root, StockPlant& stock, int child_assembly_id, std::vector<int> path);

	void changeHash(DBAdapter* lpDBAdapter, StockPlant& root, std::vector<int> path);

	int saveRelationToDB(DBAdapter* lpDBAdapter, int parent_id, int child_id);

	void setPlant(json j);

	friend void to_json(json& j, const Plant& m);

	friend void from_json(const json& j, Plant& m);
};


void to_json(json& j, const Plant& m);

void from_json(const json& j, Plant& m);
