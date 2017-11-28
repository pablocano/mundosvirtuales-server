#pragma once

#include "DBAdapter.h"
#include "Row.h"

#include <string>

using namespace db;

/// <summary>
///
/// </summary>
class ObjectDB
{
private:
	int m_id; /* Identified of register in table. */
	std::string m_tableName; /* Name of table in Database. */
	DBAdapter* m_lpDBAdapter; /* Pointer to Database functions. */

public:

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="id"></param>
	/// <param name="tableName"></param>
	/// <param name="lpDBAdapter"></param>
	ObjectDB(int id, const std::string tableName, DBAdapter* lpDBAdapter) : m_id(id), m_tableName(tableName), m_lpDBAdapter(lpDBAdapter) {}

	virtual ~ObjectDB() {};

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	int getID() const;

	/// <summary>
	///
	/// </summary>
	/// <param name="id"></param>
	virtual void setID(int id);

	bool isValidID();

	/// <summary>
	///
	/// </summary>
	/// <param name="row"></param>
	virtual void setID(const Row &row);

	/// <summary>
	///
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	virtual void setDBAdapter(DBAdapter* lpDBAdapter);

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	std::string getTableName() const;

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	Row getRowFromDB() const;

	/// <summary>
	/// Load properties this object from database.
	/// </summary>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	virtual bool loadFromDB();

	/// <summary>
	/// Load properties this object from database.
	/// </summary>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	bool loadFromDB(int id);

	/// <summary>
	///
	/// </summary>
	/// <param name="row"></param>
	/// <returns></returns>
	bool saveToDB(Row& row);

	/// <summary>
	///
	/// </summary>
	void deleteToDB();

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	virtual bool saveToDB();

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	virtual std::string getIDFieldName() const;

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	virtual std::string getWhere() const;

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	virtual std::string getFieldsSelect() const;

	/// <summary>
	///
	/// </summary>
	/// <returns></returns>
	virtual std::string getJoin() const;

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Returns a row constructed from this object's data.</returns>
	virtual Row getRow() const;

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	virtual void operator=(const Row& row)
	{
		setID(row);
	}

protected:
	DBAdapter* getDBAdapter() const
	{
		return m_lpDBAdapter;
	}
};
