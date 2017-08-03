#include "ObjectDB.h"

ObjectDB::ObjectDB(const Row & row)
{
	*this = row;
}

int ObjectDB::getID() const
{
	return m_id;
}

void ObjectDB::setID(int id)
{
	m_id = id;
}

bool ObjectDB::isValidID()
{
	return getID() > 0;
}

void ObjectDB::setID(const Row & row)
{
	m_id = row.get<int>(getIDFieldName());
	if (m_id <= 0)
	{
		m_id = row.get<int>(m_tableName + "_id");
	}
}

void ObjectDB::setDBAdapter(DBAdapter * lpDBAdapter)
{
	m_lpDBAdapter = lpDBAdapter;
}

std::string ObjectDB::getTableName() const
{
	return m_tableName;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>

Row ObjectDB::getRowFromDB() const
{
	std::string query = "SELECT " + getFieldsSelect() + " FROM " + m_tableName + " " + getJoin() + " WHERE " + getWhere() + ";";
	Rows rows = m_lpDBAdapter->query(query);
	if (rows.isEmpty())
	{
		Row row;
		row.setFieldData(rows.getFields());
		return row;
	}
	else
		return rows.front();
}

/// <summary>
/// Load properties this object from database.
/// </summary>
/// <returns>Returns true if the load was successful, false otherwise.</returns>

bool ObjectDB::loadFromDB()
{
	try
	{
		if (getID() > 0)
		{
			*this = getRowFromDB();
			return true;
		}
		return false;
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("ObjectDB", e.what());
		return false;
	}
}

/// <summary>
/// Load properties this object from database.
/// </summary>
/// <returns>Returns true if the load was successful, false otherwise.</returns>

bool ObjectDB::loadFromDB(int id)
{
	setID(id);
	return loadFromDB();
}

/// <summary>
/// 
/// </summary>
/// <param name="row"></param>
/// <returns></returns>

bool ObjectDB::saveToDB(Row & row)
{
	try
	{
		if (!row.isEmpty())
		{
			std::string where = getWhere();
			if (getID() > 0 && m_lpDBAdapter->countQuery(m_tableName, where) > 0)
			{
				return m_lpDBAdapter->update(m_tableName, row, where);
			}
			else
			{
				int id = m_lpDBAdapter->insert(m_tableName, row);
				setID(id);
				return getID() > 0;
			}
		}
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("ObjectDB", e.what());
	}

	return false;
}

void ObjectDB::deleteToDB()
{
	std::string query = "DELETE " + getFieldsSelect() + " FROM " + m_tableName + " WHERE " + getWhere() + ";";
	m_lpDBAdapter->onlyQuery(query);
	setID(0);
}

bool ObjectDB::saveToDB()
{
	Row row = getRow();
	return saveToDB(row);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>

std::string ObjectDB::getIDFieldName() const
{
	return m_tableName + "." + m_tableName + "_id";
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>

std::string ObjectDB::getWhere() const
{
	return getIDFieldName() + " = " + std::to_string(m_id);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>

std::string ObjectDB::getFieldsSelect() const
{
	return "*"; // Gets all fields
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>

std::string ObjectDB::getJoin() const
{
	return "";
}

/// <summary>
/// Gets a row.
/// </summary>
/// <returns>Returns a row constructed from this object's data.</returns>

Row ObjectDB::getRow() const
{
	return Row();
}
