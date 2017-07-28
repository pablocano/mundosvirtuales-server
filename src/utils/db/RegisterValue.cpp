#include "RegisterValue.h"

#include <ctime>
#include <time.h>

using namespace db;

RegisterValue::RegisterValue() : m_value(nullptr), m_size(0), m_indicator(IndicatorField::IS_OK), m_fieldData() {}

RegisterValue::RegisterValue(const RegisterValue & w)
{
	m_size = w.m_size;
	m_value = malloc(m_size);
	m_indicator = w.m_indicator;
	m_fieldData = w.m_fieldData;
	if (m_value && w.m_value)
		memcpy(m_value, w.m_value, m_size);
	else
		m_value = nullptr;
}

RegisterValue::RegisterValue(RegisterValue && w)
{
	m_size = std::move(w.m_size);
	m_value = std::move(w.m_value);
	m_indicator = std::move(w.m_indicator);
	m_fieldData = std::move(w.m_fieldData);
	w.m_value = nullptr;
	w.m_size = 0;
}

RegisterValue& RegisterValue::operator=(const RegisterValue& w)
{
	m_size = w.m_size;
	m_value = malloc(m_size);
	m_indicator = w.m_indicator;
	m_fieldData = w.m_fieldData;
	if (m_value && w.m_value)
		memcpy(m_value, w.m_value, m_size);
	else
		m_value = nullptr;
	return *this;
}

RegisterValue::~RegisterValue()
{
	if (m_value)
		free(m_value);
}

IndicatorField db::RegisterValue::getIndicator() const 
{
	return m_indicator;
}

void RegisterValue::setIndicator(IndicatorField indicator) 
{ 
	m_indicator = indicator;
}

const FieldData& RegisterValue::getFieldData() 
{ 
	return *m_fieldData;
}

void RegisterValue::setFieldData(const FieldData& field)
{ 
	m_fieldData = &field;
}

std::string RegisterValue::getSQLValue() const
{
	std::string value;
	switch (m_fieldData->getType())
	{
	case TypeData::DB_BOOL:
		value = get<bool>() ? "TRUE" : "FALSE";
		break;
	case TypeData::DB_STRING:
		value = "'" + get<std::string>() + "'";
		break;
	case TypeData::DB_DOUBLE:
		value = std::to_string(get<double>());
		break;
	case TypeData::DB_INTEGER:
		value = std::to_string(get<int>());
		break;
	case TypeData::DB_UNSIGNED_LONG:
		value = std::to_string(get<unsigned long>());
		break;
	case TypeData::DB_LONG_LONG:
		value = std::to_string(get<long long>());
		break;
	case TypeData::DB_DATE:
		char str[100];
		asctime_s(str, sizeof(str), &get<std::tm>());
		value = "'" + std::string(str) + "'";
		break;
	default:
		return std::string("NULL");
	}

	return value;
}