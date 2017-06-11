#pragma once

#include "utils/network/ServerTCP.h"

#include <mutex>
#include <condition_variable>

class ServerPlant : public ServerTCP
{
public:
	ServerPlant() : ServerTCP("proter", "postgres", "127.0.0.1", 5432, "test123") {}
};
