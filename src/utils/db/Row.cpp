#include "Row.h"
#include "Row.h"

#include <sstream>

using namespace db;

Row::Row() : m_lpFields(nullptr), m_registers() {}

Row::Row(const Row & row)
{
	*this = row;
}

Row& Row::operator=(const Row& row)
{
	m_lpFields = row.m_lpFields;
	m_registers = row.m_registers;
	return *this;
}

void Row::addRegister(RegisterValue& registerValue)
{
	registerValue.setFieldData(m_lpFields->at(m_registers.size()));
	m_registers.push_back(registerValue);
}

void Row::setFieldData(std::shared_ptr<Fields> fields)
{
	m_lpFields = fields;
}

int Row::find_field(std::string fieldName) const
{
	auto it = std::find(m_lpFields->begin(), m_lpFields->end(), fieldName);
	if (it == m_lpFields->end())
		return -1;
	else
		return (int)std::distance(m_lpFields->begin(), it);
}

bool db::Row::isEmpty() const
{
	return m_registers.size() <= 0;
}
