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
		bool m_isPrimaryKey;

	public:
		FieldData(std::string name = "", TypeData type = TypeData::DB_NONE, bool isPrimaryKey = false);

		std::string getName() const;

		void setName(std::string name);

		TypeData getType() const;

		void setType(TypeData type);

		bool getIsPrimaryKey() const;

		void setIsPrimaryKey(bool isPrimaryKey);

		bool operator==(const FieldData field);

		bool operator==(const std::string name);
	};

	class Fields
	{
		std::vector<FieldData> m_fields;
	public:
		Fields() : m_fields() {}

		void push_back(FieldData& field);

		FieldData& front();

		FieldData& back();

		std::vector<FieldData>::iterator begin();

		std::vector<FieldData>::iterator end();

		std::vector<FieldData>::const_iterator cbegin() const;

		std::vector<FieldData>::const_iterator cend() const;

		const FieldData& at(std::size_t pos) const;

		const FieldData& operator[](std::size_t pos) const;

		int size() const;

		const FieldData& getPrimaryKey();
	};
}