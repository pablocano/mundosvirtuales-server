#include "DBAdapter.h"

#include <iostream>
#include <cstring>
#include <sstream>
#include <ctime>

#include "../logger/Logger.h"

using namespace db;

DBAdapter::DBAdapter(std::string _db_name, std::string _db_user, std::string _db_host, int _db_port, std::string _db_password, std::string _db_engine) :
	db_name(_db_name), db_user(_db_user), db_host(_db_host), db_port(_db_port), db_password(_db_password), db_engine(_db_engine),
	current_language_id(1) // Default language
{
}

std::string DBAdapter::get_str_connection() const
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

void DBAdapter::set_language(std::string lang)
{
	current_language_id = 1; // TODO: select language from lang argument
}

std::string DBAdapter::get_text_from_path(std::string path_file) const
{
	return path_file; // TODO: this function must return the content of file.
}