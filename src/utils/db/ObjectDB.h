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

public:

	/// <summary>
	/// 
	/// </summary>
	/// <param name="id"></param>
	/// <param name="tableName"></param>
	ObjectDB(int id, const std::string tableName) : m_id(id), m_tableName(tableName) {}

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
	/// <param name="lpDBAdapter"></param>
	/// <returns></returns>
	Row getRowFromDB(DBAdapter* lpDBAdapter) const
	{
		Rows rows = lpDBAdapter->query("SELECT " + getFieldsSelect() + " from " + m_tableName + " " + getJoin() + " where " + getWhere() + ";");
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
protected:
	typedef std::vector<T> m_objects;

public:
	VectorObjectDB() : m_objects() {}

	bool loadFromDB(DBAdapter* lpDBAdapter)
	{
		try
		{
			for (auto obj = m_object.begin(); obj != m_objects.end(); ++obj)
			{
				*obj = obj.getRowFromDB(lpDBAdapter);
			}
			return true;
		}
		catch (const std::exception &e)
		{
			LOGGER_ERROR("ObjectDB", e.what());
			return false;
		}
	}

	bool loadFromDB(DBAdapter* lpDBAdapter, std::vector<int> ids)
	{
		try
		{
			for (int id : ids)
			{
				T obj;
				obj.setID(id);
				obj.loadFromDB(lpDBAdapter);
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

	void push_back(T& obj) 
	{ 
		m_objects.push_back(obj);
	}

	T& front() const
	{
		return m_objects.front();
	}

	T& back() const
	{
		return m_objects.back();
	}

	std::vector<T>::iterator begin()
	{
		return m_objects.begin();
	}

	std::vector<T>::iterator end()
	{
		return m_objects.end();
	}

	std::vector<T>::const_iterator cbegin() const
	{
		return m_objects.cbegin();
	}

	std::vector<T>::const_iterator cend() const
	{
		return m_objects.cend();
	}

	int size() const
	{
		return (int) m_objects.size();
	}
};