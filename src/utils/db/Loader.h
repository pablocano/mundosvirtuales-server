#pragma once

#include "DB.h"
#include "../../plant/Machine.h"

using namespace db;

class Loader
{
protected:
	DB m_dataBase;

public:
	Loader(std::string _db_name, std::string _db_user,  std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");
	
	Machines load_machines();

};