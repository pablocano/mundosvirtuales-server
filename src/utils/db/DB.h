#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstring>

namespace db
{
	enum IndicatorField
	{
		IS_OK,
		IS_NULL,
		IS_TRUNCATED
	};

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
		FieldData(std::string name = "", TypeData type = TypeData::NONE) : m_name(name), m_type(type) {}

		std::string getName() const { return m_name; }

		void setName(std::string name) { m_name = name; }

		TypeData getType() const { return m_type; }

		void setType(TypeData type) { m_type = type; }

		bool operator==(const FieldData field)
		{
			return (m_name == field.m_name) && (m_type == field.m_type);
		}
	};

	class RegisterValue
	{
	protected:
		void* m_value;
		int m_size;
		IndicatorField m_indicator;
		FieldData* m_fieldData;

	public:

		RegisterValue() : m_value(nullptr), m_size(0), m_indicator(IndicatorField::IS_OK), m_fieldData(nullptr) {}

		RegisterValue(RegisterValue& w)
		{
			m_size = w.m_size;
			if (m_value)
				m_value = realloc(m_value, m_size);
			else
				m_value = malloc(m_size);
			m_indicator = w.m_indicator;
			m_fieldData = w.m_fieldData;
			memcpy(m_value, w.m_value, m_size);
		}

		RegisterValue(RegisterValue&& w)
		{
			if (m_value) free(m_value);
			m_size = std::move(w.m_size);
			m_value = std::move(w.m_value);
			m_indicator = std::move(w.m_indicator);
			m_fieldData = std::move(w.m_fieldData);
			w.m_value = nullptr;
			w.m_size = 0;
		}

		RegisterValue& operator=(const RegisterValue& w)
		{
			m_size = w.m_size;
			if (m_value)
				m_value = realloc(m_value, m_size);
			else
				m_value = malloc(m_size);
			m_indicator = w.m_indicator;
			m_fieldData = w.m_fieldData;
			memcpy(m_value, w.m_value, m_size);
			return *this;
		}

		~RegisterValue() { if (m_value) free(m_value); }

		template<typename T>
		void set(const T& p)
		{
			m_size = sizeof(T);
			if (m_value)
			{
				m_value = realloc(m_value, m_size);
			}
			else
			{
				m_value = malloc(m_size);
			}
			memcpy(m_value, static_cast<const void *>(&p), m_size);
		}

		template<typename T>
		T& get() const
		{
			T& d = *static_cast<T*>(m_value);
			return d;
		}

		IndicatorField getIndicator() const { return m_indicator; }

		void setIndicator(IndicatorField indicator) { m_indicator = indicator; }

		FieldData getFieldData() { return *m_fieldData; }

		void setFieldData(FieldData* field) { m_fieldData = field; }
	};

	class Row
	{
	protected:
		std::vector<FieldData>& m_fields;
		std::vector<RegisterValue> m_registers;

	public:
		Row(std::vector<FieldData>& fields) : m_fields(fields), m_registers() {}

		void addRegister(const RegisterValue& registerValue);

		template<typename T>
		T get(std::string field, T default = T()) const
		{
			int pos = find_field(field);
			if (pos < 0)
				return default;
			else
				return m_registers[pos].get<T>();
		}

		int find_field(std::string fieldName) const;
	};

	class Rows : public std::vector<Row>
	{
	public:
		Rows() : m_fields() {}

		void addField(FieldData field);

		std::vector<FieldData>& getFields() { return m_fields; }

	private:
		std::vector<FieldData> m_fields;
	};

	class DB
	{
	protected:
		std::string db_user;
		std::string db_password;
		std::string db_engine;
		std::string db_name;
		std::string db_host;
		int db_port;

		int current_language_id;

	public:
		DB(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");

		void set_language(std::string lang);

		Rows query(std::string query) const;

		int countQuery(std::string table, std::string where) const;

	private:
		std::string get_text_from_path(std::string path_file) const;

		std::string get_str_connection() const; /* returns string connection for library SOCI */

	};
}
