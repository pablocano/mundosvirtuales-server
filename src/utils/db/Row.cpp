#include "Row.h"
#include "Row.h"

#include <sstream>

using namespace db;

Row::Row() : m_lpFields(nullptr), m_registers() {}

Row::Row(const Row & row)
{
	m_lpFields = row.m_lpFields;
	m_registers = row.m_registers;
}

Row& Row::operator=(const Row& row)
{
	m_lpFields = row.m_lpFields;
	m_registers = row.m_registers;
	return *this;
}

void Row::addRegister(RegisterValue& registerValue)
{
	registerValue.setFieldData(&m_lpFields->at(m_registers.size()));
	m_registers.push_back(registerValue);
}

void Row::setFieldData(Fields* fields)
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

std::string Row::getSQLFieldNames() const
{
	std::stringstream ss;
	ss.str("");
	ss << "(";
	int pos = 0;
	for (; pos < m_lpFields->size() - 1; ++pos)
		ss << m_lpFields->at(pos).getName() << ", ";

	if (pos == m_lpFields->size() - 1)
		ss << m_lpFields->at(pos).getName();

	ss << ")";

	return ss.str();
}

std::string Row::getSQLRegisterValues() const
{
	std::stringstream ss;
	ss.str("");
	ss << "(";
	int pos = 0;
	for (; pos < m_registers.size() - 1; ++pos)
		ss << m_registers.at(pos).getSQLValue() << ", ";

	if (pos == m_registers.size() - 1)
		ss << m_registers.at(pos).getSQLValue();

	ss << ")";

	return ss.str();
}

std::string Row::getSQLUpdateRegisterValues() const
{
	std::stringstream ss;
	ss.str("");
	int pos = 0;
	for (; pos < m_registers.size() - 1; ++pos)
		ss << m_lpFields->at(pos).getName() << " = " << m_registers.at(pos).getSQLValue() << ", ";

	if (pos == m_registers.size() - 1)
		ss << m_lpFields->at(pos).getName() << " = " << m_registers.at(pos).getSQLValue();

	return ss.str();
}
