#pragma once

#include <string>

namespace db
{
	/// <summary>
	/// 
	/// </summary>
	enum TypeData
	{
		NONE,
		STRING,
		INTEGER,
		DOUBLE,
		UNSIGNED_LONG,
		LONG_LONG,
		DATE
	};

	class FieldData
	{
	protected:
		std::string m_name;
		TypeData m_type;

	public:
		FieldData(std::string name = "", TypeData type = TypeData::NONE);

		std::string getName() const;

		void setName(std::string name);

		TypeData getType() const;

		void setType(TypeData type);

		bool operator==(const FieldData field);

		bool operator==(const std::string name);
	};
}