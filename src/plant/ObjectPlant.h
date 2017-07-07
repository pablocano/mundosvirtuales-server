#pragma once

#include "../utils/db/DBAdapter.h"
#include "../utils/db/Row.h"

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

	virtual bool loadFromDB(DBAdapter* lpDBAdapter, int id) = 0;

	virtual bool saveToDB(DBAdapter* lpDBAdapter) = 0;

	virtual void operator=(const Row& row) = 0;

	Row getRow() { return Row(); }
};

