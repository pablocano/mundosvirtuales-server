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
	/// 
	/// </summary>
	/// <param name="id"></param>
	/// <param name="tableName"></param>
	ObjectDB(int id, const std::string tableName, DBAdapter* lpDBAdapter) : m_id(id), m_tableName(tableName), m_lpDBAdapter(lpDBAdapter) {}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	int getID() const 
	{ 
		return m_id;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	void setID(int id) 
	{ 
		m_id = id;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="row"></param>
	void setID(const Row &row) 
	{ 
		m_id = row.get<int>(getIDFieldName());
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="lpDBAdapter"></param>
	void setDBAdapter(DBAdapter* lpDBAdapter)
	{
		m_lpDBAdapter = lpDBAdapter;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getIDFieldName() const
	{
		return m_tableName + "_id = " + std::to_string(m_id);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getTableName() const 
	{ 
		return m_tableName;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	Row getRowFromDB() const
	{
		Rows rows = m_lpDBAdapter->query("SELECT " + getFieldsSelect() + " from " + m_tableName + " " + getJoin() + " where " + getWhere() + ";");
		return rows.front();
	}

	/// <summary>
	/// Load properties this object from database.
	/// </summary>
	/// <returns>Returns true if the load was successful, false otherwise.</returns>
	bool loadFromDB()
	{
		try
		{
			*this = getRowFromDB();
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
	/// <returns>Returns true if this object was saved successfully, false otherwise.</returns>
	bool saveToDB()
	{
		try
		{
			Row row = getRow();
			std::string where = getWhere();
			if (m_lpDBAdapter->countQuery(m_tableName, where) > 0)
			{
				return m_lpDBAdapter->update(m_tableName, row, where);
			}
			else
			{
				return m_lpDBAdapter->insert(m_tableName, row);
			}
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("ObjectDB", e.what());
			return false;
		}
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getWhere() const
	{
		return m_tableName + "_id = " + std::to_string(m_id);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getFieldsSelect() const
	{
		return "*"; // Gets all fields
	}

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	std::string getJoin() const
	{
		return "";
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

template<typename T>
class VectorObjectDB
{
static_assert(std::is_base_of<ObjectDB, T>::value, "T must derive from ObjectDB");

protected:
	std::vector<std::shared_ptr<T>> m_objects;

public:
	VectorObjectDB() : m_objects() {}

	bool loadFromDB()
	{
		try
		{
			for (std::shared_ptr<T> obj : m_objects)
			{
				obj->loadFromDB();
			}
			return true;
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("ObjectDB", e.what());
			return false;
		}
	}

	bool loadFromDB(DBAdapter* lpDBAdapter, std::string table, std::vector<int> ids)
	{
		try
		{
			for (int id : ids)
			{
				std::shared_ptr<ObjectDB> obj(new T(id, table, lpDBAdapter));
				if(obj->loadFromDB())
					push_back(obj);
			}
			return true;
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("VectorObjectDB", e.what());
			return false;
		}
	}

	bool saveToDB()
	{
		try
		{
			for (std::shared_ptr<T> obj : m_objects)
			{
				obj->saveToDB();
			}
			return true;
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("ObjectDB", e.what());
			return false;
		}
	}

	void push_back(std::shared_ptr<T>& obj)
	{ 
		m_objects.push_back(obj);
	}

	std::shared_ptr<T> front() const
	{
		return m_objects.front();
	}

	std::shared_ptr<T> back() const
	{
		return m_objects.back();
	}

	auto begin()
	{
		return m_objects.begin();
	}

	auto end()
	{
		return m_objects.end();
	}

	auto cbegin() const
	{
		return m_objects.cbegin();
	}

	auto cend() const
	{
		return m_objects.cend();
	}

	int size() const
	{
		return (int) m_objects.size();
	}
};