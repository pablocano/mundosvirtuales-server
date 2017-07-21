#include "Rows.h"

using namespace db;

Rows::Rows() : m_fields(), m_rows() {}

Rows::Rows(const Rows & rows)
{
	m_fields = rows.m_fields;
	m_rows = rows.m_rows;
	setFields();
}

Rows& Rows::operator=(const Rows& rows)
{
	m_fields = rows.m_fields;
	m_rows = rows.m_rows;
	setFields();

	return *this;
}

void Rows::setFields()
{
	for (auto it = m_rows.begin(); it != m_rows.end(); ++it)
	{
		it->setFieldData(m_fields);
	}
}

void Rows::push_back(Row & row) 
{ 
	m_rows.push_back(row); 
	m_rows.back().setFieldData(m_fields);
}

Row& Rows::front()
{
	return m_rows.front();
}

Row& Rows::back() 
{ 
	return m_rows.back();
}

std::vector<Row>::iterator Rows::begin() 
{ 
	return m_rows.begin();
}

std::vector<Row>::iterator Rows::end() 
{ 
	return m_rows.end();
}

std::vector<Row>::const_iterator Rows::cbegin() const 
{ 
	return m_rows.cbegin();
}

std::vector<Row>::const_iterator Rows::cend() const
{
	return m_rows.cend();
}

int Rows::size() const
{
	return (int) m_rows.size();
}

void Rows::addField(FieldData field)
{
	m_fields->push_back(field);
}

std::shared_ptr<Fields> Rows::getFields()
{ 
	return m_fields;
}
