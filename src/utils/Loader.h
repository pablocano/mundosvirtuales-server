#pragma once

#include "../plant/Machine.h"

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
	/* Tables' names */
	const std::string tbl_language				= "language";
	const std::string tbl_machines				= "machines";
	const std::string tbl_machine_translation	= "machinetranslation";
	const std::string tbl_models				= "models";
	const std::string tbl_parts_of_machine		= "partsofmachine";
	const std::string tbl_parts_of_model		= "partsofmodel";
	const std::string tbl_position_model		= "positionmodel";
	const std::string tbl_stock					= "stock";
	const std::string tbl_stock_translation		= "stocktranslation";

public:
	Loader(std::string _db_name, std::string _db_user,  std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");

	std::string get_str_connection(); /* returns string connection for library SOCI */

	void set_language(std::string lang);
	
	Machines load_machines();

protected:
	std::string get_text_from_path(std::string path_file);

};