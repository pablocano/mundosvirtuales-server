#include "DBAdapterSOCI.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <ctime>
#include <regex>

#include "soci/soci.h"
#include "soci/soci-config.h"
#include "soci/postgresql/soci-postgresql.h"

#include "../logger/Logger.h"

using namespace db;

DBAdapterSOCI::DBAdapterSOCI(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	DBAdapter(_db_name, _db_user, _db_host, _db_port, _db_password, _db_engine)
{
}

void DBAdapterSOCI::onlyQuery(std::string query) const
{
	std::string connectString = get_str_connection();
	soci::session sql(db_engine, connectString);

	sql << query;
}

Rows DBAdapterSOCI::query(std::string query) const
{
	Rows rows;

	std::string connectString = get_str_connection();
	soci::session sql(db_engine, connectString);

	soci::row r;
	sql << query, soci::into(r);

	for (std::size_t i = 0; i != r.size(); ++i)
	{
		TypeData type = TypeData::DB_NONE;

		const soci::column_properties & props = r.get_properties(i);

		switch (props.get_data_type())
		{
		case soci::dt_string:
			type = TypeData::DB_STRING;
			break;
		case soci::dt_double:
			type = TypeData::DB_DOUBLE;
			break;
		case soci::dt_integer:
			type = TypeData::DB_INTEGER;
			break;
		case soci::dt_unsigned_long_long:
			type = TypeData::DB_UNSIGNED_LONG;
			break;
		case soci::dt_long_long:
			type = TypeData::DB_LONG_LONG;
			break;
		case soci::dt_date:
			type = TypeData::DB_DATE;
			break;
		}

		rows.addField(FieldData(props.get_name(), type));
	}

	soci::rowset<soci::row> soci_rows(sql.prepare << query);

	for (auto it = soci_rows.begin(); it != soci_rows.end(); ++it)
	{
		rows.push_back(Row()); // New row
		Row& row = rows.back(); // Get last added row 

		for (std::size_t i = 0; i < it->size(); ++i)
		{
			const soci::column_properties & props = it->get_properties(i);
			IndicatorField indicator = (it->get_indicator(i) == soci::i_null) ? IndicatorField::IS_NULL : (it->get_indicator(i) == soci::i_truncated) ? IndicatorField::IS_TRUNCATED : IndicatorField::IS_OK;
			RegisterValue registerValue;

			switch ((*rows.getFields())[i].getType())
			{
			case TypeData::DB_STRING:
				registerValue.set<std::string>(it->get<std::string>(i));
				break;
			case TypeData::DB_DOUBLE:
				registerValue.set<double>(it->get<double>(i));
				break;
			case TypeData::DB_INTEGER:
				registerValue.set<int>(it->get<int>(i));
				break;
			case TypeData::DB_BOOL:
				registerValue.set<int>(it->get<int>(i) != 0);
				break;
			case TypeData::DB_UNSIGNED_LONG:
				registerValue.set<unsigned long>(it->get<unsigned long>(i));
				break;
			case TypeData::DB_LONG_LONG:
				registerValue.set<long long>(it->get<long long>(i));
				break;
			case TypeData::DB_DATE:
				registerValue.set<std::tm>(it->get<std::tm>(i));
				break;
			}

			registerValue.setIndicator(indicator);
			registerValue.setFieldData(rows.getFields()->at(i));
			row.addRegister(registerValue);
		}
	}

	return rows;
}

int DBAdapterSOCI::countQuery(std::string table, std::string where) const
{
	int counts = 0;
	std::stringstream strSQL;

	std::string connectString = get_str_connection();
	soci::session sql(db_engine, connectString);

	strSQL.str("");
	strSQL << "SELECT count(*) FROM " << table << " WHERE " << where;
	sql << strSQL.str(), soci::into(counts);

	return counts;
}

std::vector<int> DBAdapterSOCI::insert(const std::string& table, const Rows & rows)
{
	std::vector<int> ids(rows.size(), -1);
	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);

		std::vector<int> skip_pos;
		int pos_primary_key = getPos(rows.getFields(), rows.getFields()->getPrimaryKey().getName());
		skip_pos.push_back(pos_primary_key);

		soci::transaction tr(sql);

		for (auto row = rows.cbegin(); row != rows.cend(); ++row)
		{
			std::stringstream ss;
			ss << "INSERT INTO " << table;
			if (pos_primary_key > 0 && row->getRegisters().at(pos_primary_key).get<int>() <= 0)
			{
				ss << getSQLFieldNames(*row, skip_pos) << " VALUES" << getSQLRegisterValues(*row, skip_pos);
			}
			else
			{
				ss << getSQLFieldNames(*row) << " VALUES" << getSQLRegisterValues(*row);
			}
			
			ss << " RETURNING " << table << "_id" << ";";
			std::regex pattern(table + "\\.");
			std::string query = ss.str();
			int id;
			sql << std::regex_replace(query, pattern, ""), soci::into(id);
			ids.push_back(id);
		}

		tr.commit();
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("DBAdapterSOCI", e.what());
	}

	return ids;
}

int DBAdapterSOCI::insert(const std::string& table, const Row & row)
{
	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);

		std::vector<int> skip_pos;
		int pos_primary_key = getPos(row.getFields(), row.getFields()->getPrimaryKey().getName());
		skip_pos.push_back(pos_primary_key);

		std::stringstream ss;
		ss << "INSERT INTO " << table;
		if (pos_primary_key >= 0 && row.getRegisters().at(pos_primary_key).get<int>() <= 0)
		{
			ss << getSQLFieldNames(row, skip_pos) << " VALUES" << getSQLRegisterValues(row, skip_pos);
		}
		else
		{
			ss << getSQLFieldNames(row) << " VALUES" << getSQLRegisterValues(row);
		}

		ss << " RETURNING " << table << "_id" << ";";
		std::regex pattern(table + "\\.");
		std::string query = ss.str();
		int id;
		sql << std::regex_replace(query, pattern, ""), soci::into(id);

		return id;
	}
	catch(const std::exception &e)
	{
		LOGGER_ERROR("DBAdapterSOCI", e.what());
		return -1;
	}
}

bool db::DBAdapterSOCI::update(const std::string & table, const Rows & rows, const std::string name_id)
{
	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);
		
		std::regex pattern(table + "\\.");

		soci::transaction tr(sql);

		for (auto row = rows.cbegin(); row != rows.cend(); ++row)
		{
			std::stringstream ss;
			ss.str("");
			ss << "UPDATE " << table << " SET " << getSQLUpdateRegisterValues(*row) << " WHERE " << name_id << " = " << row->get<int>(name_id) << ";";
			
			std::string query = ss.str();
			sql << std::regex_replace(query, pattern, "");
		}

		tr.commit();
		return true;
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("DBAdapterSOCI", e.what());
		return false;
	}
}

bool db::DBAdapterSOCI::update(const std::string & table, const Row & row, const std::string where)
{
	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);

		std::stringstream ss;
		ss.str("");
		ss << "UPDATE " << table << " SET " << getSQLUpdateRegisterValues(row) << " WHERE " << where << ";";

		std::regex pattern(table + "\\.");
		std::string query = ss.str();
		sql << std::regex_replace(query, pattern, "");

		return true;
	}
	catch (const std::exception &e)
	{
		LOGGER_ERROR("DBAdapterSOCI", e.what());
		return false;
	}
}
