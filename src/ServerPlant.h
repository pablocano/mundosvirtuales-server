#pragma once

#include "utils/network/ServerTCP.h"
#include "utils/db/DBAdapter.h"

#include <mutex>
#include <condition_variable>

class ServerPlant : public ServerTCP
{
public:
	ServerPlant(DBAdapter* lpDBAdapter) : ServerTCP(lpDBAdapter) {}
};
