#include "FieldData.h"

using namespace db;

FieldData::FieldData(std::string name, TypeData type, bool isPrimaryKey) : m_name(name), m_type(type), m_isPrimaryKey(isPrimaryKey) {}

std::string FieldData::getName() const 
{ 
	return m_name;
}

void FieldData::setName(std::string name) 
{ 
	m_name = name;
}

TypeData FieldData::getType() const 
{
	return m_type;
}

void FieldData::setType(TypeData type) 
{
	m_type = type;
}

bool db::FieldData::getIsPrimaryKey() const
{
	return m_isPrimaryKey;
}

void db::FieldData::setIsPrimaryKey(bool isPrimaryKey)
{
	m_isPrimaryKey = isPrimaryKey;
}

bool FieldData::operator==(const FieldData field)
{
	return (m_name == field.m_name) && (m_type == field.m_type);
}

bool FieldData::operator==(const std::string name)
{
	return m_name == name;
}

void Fields::push_back(FieldData & field)
{
	m_fields.push_back(field);
}

FieldData & Fields::front()
{
	return m_fields.front();
}

FieldData & Fields::back()
{
	return m_fields.back();
}

std::vector<FieldData>::iterator Fields::begin()
{
	return m_fields.begin();
}

std::vector<FieldData>::iterator Fields::end()
{
	return m_fields.end();
}

std::vector<FieldData>::const_iterator Fields::cbegin() const
{
	return m_fields.cbegin();
}

std::vector<FieldData>::const_iterator Fields::cend() const
{
	return m_fields.cend();
}

const FieldData& Fields::at(std::size_t pos) const
{
	return m_fields.at(pos);
}

const FieldData & Fields::operator[](std::size_t pos) const
{
	return m_fields[pos];
}

int Fields::size() const
{
	return (int) m_fields.size();
}

const FieldData & Fields::getPrimaryKey()
{
	for (const FieldData& field : *this)
	{
		if (field.getIsPrimaryKey())
			return field;
	}

	static FieldData fieldDefault;
	return fieldDefault;
}
