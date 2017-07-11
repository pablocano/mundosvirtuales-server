#pragma once

#include <string>
#include <vector>

namespace db
{
	/// <summary>
	/// Type storage data into field.
	/// </summary>
	enum TypeData
	{
		DB_NONE,
		DB_STRING,
		DB_INTEGER,
		DB_BOOL,
		DB_DOUBLE,
		DB_UNSIGNED_LONG,
		DB_LONG_LONG,
		DB_DATE
	};

	class FieldData
	{
	protected:
		std::string m_name;
		TypeData m_type;

	public:
		FieldData(std::string name = "", TypeData type = TypeData::DB_NONE);

		std::string getName() const;

		void setName(std::string name);

		TypeData getType() const;

		void setType(TypeData type);

		bool operator==(const FieldData field);

		bool operator==(const std::string name);
	};

	typedef std::vector<FieldData> Fields;
}