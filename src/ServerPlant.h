#pragma once

#include "utils/network/ServerTCP.h"

#include <mutex>
#include <condition_variable>

class ServerPlant : public ServerTCP
{
public:
	ServerPlant(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql") : ServerTCP(_db_name, _db_user, _db_host, _db_port, _db_password, _db_engine) {}
};
