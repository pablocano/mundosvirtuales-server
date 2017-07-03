#pragma once

#include "../../plant/Assembly.h"
#include "DB.h"

using namespace db;

class Loader
{
protected:
	DB m_dataBase;

public:
	Loader(std::string _db_name, std::string _db_user,  std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");
	
	Assemblies load_machines();

};