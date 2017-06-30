#pragma once

#include "../../plant/Assembly.h"

class Loader
{
protected:
	std::string db_user;
	std::string db_password;
	std::string db_engine;
	std::string db_name;
	std::string db_host;
	int db_port;

	int current_language_id;

	std::string strError;

public:
	Loader(std::string _db_name, std::string _db_user,  std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");

	std::string get_str_connection(); /* returns string connection for library SOCI */

	void set_language(std::string lang);
	
	Assemblies load_machines();

protected:
	std::string get_text_from_path(std::string path_file);

};