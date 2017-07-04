#include "DB.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <ctime>

#include "soci/soci.h"
#include "soci/soci-config.h"
#include "soci/postgresql/soci-postgresql.h"

#include "../logger/Logger.h"

using namespace db;

DB::DB(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	db_name(_db_name), db_user(_db_user), db_host(_db_host), db_port(_db_port), db_password(_db_password), db_engine(_db_engine),
	current_language_id(1) // Default language
{
}

std::string DB::get_str_connection() const
{
	std::stringstream ss;

	ss.str("");
	if (!db_host.empty())
	{
		ss << "host=" << db_host << " ";
		ss << "port=" << db_port << " ";
	}
	ss << "dbname=" << db_name << " ";
	ss << "user=" << db_user;
	if (!db_password.empty())
		ss << " password=" << db_password;

	return ss.str();
}

void DB::set_language(std::string lang)
{
	current_language_id = 1; // TODO: select language from lang argument
}

Rows DB::query(std::string query) const
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

int DB::countQuery(std::string table, std::string where) const
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

bool DB::insert(const std::string& table, const Rows & rows)
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

bool DB::insert(const std::string& table, const Row & row)
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
		LOGGER_ERROR("DB", e.what());
		return false;
	}
}

std::string DB::get_text_from_path(std::string path_file) const
{
	return path_file; // TODO: this function must return the content of file.
}
