#pragma once

#include "../utils/db/DBAdapter.h"

#include <string>

using namespace db;

class ObjectPlant
{
protected:
	int m_id;

public:
	ObjectPlant(int id = 0) : m_id(id) {}

	int getID() const { return m_id; }

	void setID(int id) { m_id = id; }

	virtual void loadFromDB(DBAdapter* lpDBAdapter) = 0;
};

