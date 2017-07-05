#pragma once

#include "../../plant/Assembly.h"
#include "DBAdapter.h"

using namespace db;

class Loader
{
protected:
	DBAdapter* m_lpDataBase;

public:
	Loader(DBAdapter* lpDataBase);
	
	Assemblies load_machines();

};