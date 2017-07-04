#include "FieldData.h"

using namespace db;

FieldData::FieldData(std::string name, TypeData type) : m_name(name), m_type(type) {}

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

bool FieldData::operator==(const FieldData field)
{
	return (m_name == field.m_name) && (m_type == field.m_type);
}

bool FieldData::operator==(const std::string name)
{
	return m_name == name;
}

