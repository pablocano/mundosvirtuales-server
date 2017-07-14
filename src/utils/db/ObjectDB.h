#pragma once

#include "DBAdapter.h"
#include "Row.h"

#include <string>

using namespace db;

class ObjectDB
{
private:
	int m_id;
	std::string m_tableName;

public:
	ObjectDB(int id, const std::string tableName) : m_id(id), m_tableName(tableName) {}

	int getID() const { return m_id; }

	std::string getIDFieldName() const
	{
		return m_tableName + "_id = " + std::to_string(m_id);
	}

	void setID(int id) { m_id = id; }

	void setID(const Row &row) { m_id = row.get<int>(getIDFieldName()); }

	std::string getTableName() const { return m_tableName; }

	Row getRowFromDB(DBAdapter* lpDBAdapter) const
	{
		Rows rows = lpDBAdapter->query("SELECT " + getFieldsSelect() + " from " + m_tableName + " where " + getWhere() + ";");
		return rows.front();
	}

	/// <summary>
	/// Load properties this object from database.
	/// </summary>
	/// <param name="lpDBAdapter">Pointer to the database handle.</param>
	/// <param name="id">Identifier of Assembly in the database.</param>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	bool loadFromDB(DBAdapter* lpDBAdapter)
	{
		try
		{
			*this = getRowFromDB(lpDBAdapter);
			return true;
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("ObjectDB", e.what());
			return false;
		}
	}

	/// <summary>
	/// Save Assembly to database.
	/// </summary>
	/// <param name="lpDBAdapter">Pointer to the database handle.</param>
	/// <returns>Returns true if this object was saved successfully, false otherwise.</returns>
	bool saveToDB(DBAdapter* lpDBAdapter)
	{
		try
		{
			Row row = getRow();
			std::string where = getWhere();
			if (lpDBAdapter->countQuery(m_tableName, where) > 0)
			{
				return lpDBAdapter->update(m_tableName, row, where);
			}
			else
			{
				return lpDBAdapter->insert(m_tableName, row);
			}
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("ObjectDB", e.what());
			return false;
		}
	}

	std::string getWhere() const
	{
		return m_tableName + "_id = " + std::to_string(m_id);
	}

	std::string getFieldsSelect() const
	{
		return "*"; // Gets all fields
	}

	/// <summary>
	/// Gets a row.
	/// </summary>
	/// <returns>Returns a row constructed from this object's data.</returns>
	virtual Row getRow() const = 0;

	/// <summary>
	/// Operator equals with Row.
	/// </summary>
	/// <param name="row">Row reference.</param>
	virtual void operator=(const Row& row)
	{
		setID(row);
	}
};

template<typename >
class VectorObjectDB
{
protected:
	typedef std::vector<SubAssembly> m_subAssemblies;

public:
	SubAssemblies();

	void push_back(SubAssembly& subAssembly);

	SubAssembly& front();

	SubAssembly& back();

	std::vector<SubAssembly>::iterator begin();

	std::vector<SubAssembly>::iterator end();

	std::vector<SubAssembly>::const_iterator cbegin() const;

	std::vector<SubAssembly>::const_iterator cend() const;

	int size() const;
};