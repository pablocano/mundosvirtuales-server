#include "DBAdapterSOCI.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <ctime>

#include "soci/soci.h"
#include "soci/soci-config.h"
#include "soci/postgresql/soci-postgresql.h"

#include "../logger/Logger.h"

using namespace db;

DBAdapterSOCI::DBAdapterSOCI(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	DBAdapter(_db_name, _db_user, _db_host, _db_port, _db_password, _db_engine)
{
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
		TypeData type = TypeData::NONE;

		const soci::column_properties & props = r.get_properties(i);

		switch (props.get_data_type())
		{
		case soci::dt_string:
			type = TypeData::STRING;
			break;
		case soci::dt_double:
			type = TypeData::DOUBLE;
			break;
		case soci::dt_integer:
			type = TypeData::INTEGER;
			break;
		case soci::dt_unsigned_long_long:
			type = TypeData::UNSIGNED_LONG;
			break;
		case soci::dt_long_long:
			type = TypeData::LONG_LONG;
			break;
		case soci::dt_date:
			type = TypeData::STRING;
			break;
		}

		rows.addField(FieldData(props.get_name(), type));
	}

	soci::rowset<soci::row> soci_rows(sql.prepare << query);

	for (auto it = soci_rows.begin(); it != soci_rows.end(); ++it)
	{
		rows.push_back(Row()); // New row
		Row& row = rows.back(); // Get last added row 

		for (std::size_t i = 0; i != it->size(); ++i)
		{
			const soci::column_properties & props = it->get_properties(i);
			IndicatorField indicator = (it->get_indicator(i) == soci::i_null) ? IndicatorField::IS_NULL : (it->get_indicator(i) == soci::i_truncated) ? IndicatorField::IS_TRUNCATED : IndicatorField::IS_OK;
			RegisterValue registerValue;

			switch ((*rows.getFields())[i].getType())
			{
			case TypeData::STRING:
				registerValue.set<std::string>(it->get<std::string>(i));
				break;
			case TypeData::DOUBLE:
				registerValue.set<double>(it->get<double>(i));
				break;
			case TypeData::INTEGER:
				registerValue.set<int>(it->get<int>(i));
				break;
			case TypeData::UNSIGNED_LONG:
				registerValue.set<unsigned long>(it->get<unsigned long>(i));
				break;
			case TypeData::LONG_LONG:
				registerValue.set<long long>(it->get<long long>(i));
				break;
			case TypeData::DATE:
				registerValue.set<std::tm>(it->get<std::tm>(i));
				break;
			}

			registerValue.setIndicator(indicator);
			registerValue.setFieldData(&(*rows.getFields())[i]);
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

bool DBAdapterSOCI::insert(const std::string& table, const Rows & rows)
{
	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);

		soci::transaction tr(sql);

		for (auto row = rows.cbegin(); row != rows.cend(); ++row)
		{
			std::stringstream ss;
			ss << "insert into " << table << row->getSQLFieldNames() << " values" << row->getSQLRegisterValues() << ";";
			sql << ss.str();
		}

		tr.commit();
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool DBAdapterSOCI::insert(const std::string& table, const Row & row)
{
	try
	{
		std::string connectString = get_str_connection();
		soci::session sql(db_engine, connectString);

		std::stringstream ss;
		ss << "insert into " << table << row.getSQLFieldNames() << " values" << row.getSQLRegisterValues() << ";";
		std::string query = ss.str();
		sql << query;
		return true;
	}
	catch(const std::exception &e)
	{
		LOGGER_ERROR("DBAdapter", e.what());
		return false;
	}
}