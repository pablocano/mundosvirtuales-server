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

		bool operator==(const std::string name)
		{
			return m_name == name;
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

		RegisterValue(const RegisterValue& w)
		{
			m_size = w.m_size;
			m_value = malloc(m_size);
			m_indicator = w.m_indicator;
			m_fieldData = w.m_fieldData;
			memcpy(m_value, w.m_value, m_size);
		}

		RegisterValue(RegisterValue&& w)
		{
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

		template<>
		void set<std::string>(const std::string& p)
		{
			m_size = (int) p.length() + 1;
			if (m_value)
			{
				m_value = realloc(m_value, m_size);
			}
			else
			{
				m_value = malloc(m_size);
			}
			memcpy(m_value, static_cast<const void *>(p.data()), p.length());
			*((char *)m_value + p.length()) = '\0';
		}

		template<typename T>
		T get() const
		{
			T d = *static_cast<T*>(m_value);
			return d;
		}

		template<>
		std::string get<std::string>() const
		{
			char* lpS = static_cast<char *>(m_value);
			std::string s = std::string(lpS);
			return s;
		}

		IndicatorField getIndicator() const { return m_indicator; }

		void setIndicator(IndicatorField indicator) { m_indicator = indicator; }

		FieldData getFieldData() { return *m_fieldData; }

		void setFieldData(FieldData* field) { m_fieldData = field; }

		std::string getValue() const;
	};

	class Row
	{
	protected:
		std::vector<FieldData>* m_lpFields;
		std::vector<RegisterValue> m_registers;

	public:
		
		Row() : m_lpFields(nullptr), m_registers() {}

		Row(const Row& row)
		{
			m_lpFields = row.m_lpFields;
			m_registers = row.m_registers;
		}

		Row& operator=(const Row& row)
		{
			m_lpFields = row.m_lpFields;
			m_registers = row.m_registers;
			return *this;
		}

		template<typename T>
		void addRegisterPerValue(const T& value)
		{
			RegisterValue registerValue;
			registerValue.set<T>(value);
			addRegister(registerValue);
		}

		void addRegister(RegisterValue& registerValue);

		void setFieldData(std::vector<FieldData>* fields)
		{
			m_lpFields = fields;
		}

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

		std::string getSQLFieldNames() const;

		std::string getSQLRegisterValues() const;
	};

	class Rows
	{
	protected:
		std::vector<FieldData> m_fields;
		std::vector<Row> m_rows;

		void setFields()
		{
			for (auto it = m_rows.begin(); it != m_rows.end(); ++it)
			{
				it->setFieldData(&m_fields);
			}
		}

	public:
		Rows() : m_fields(), m_rows() {}

		Rows(const Rows& rows)
		{
			m_fields = rows.m_fields;
			m_rows = rows.m_rows;
			setFields();
		}

		Rows& operator=(const Rows& rows)
		{
			m_fields = rows.m_fields;
			m_rows = rows.m_rows;
			setFields();

			return *this;
		}

		void push_back(Row& row) { m_rows.push_back(row); m_rows.back().setFieldData(&m_fields); }

		Row& back() { return m_rows.back(); }

		std::vector<Row>::iterator begin() { return m_rows.begin(); }

		std::vector<Row>::iterator end() { return m_rows.end(); }

		std::vector<Row>::const_iterator cbegin() const { return m_rows.cbegin(); }

		std::vector<Row>::const_iterator cend() const { return m_rows.cend(); }

		void addField(FieldData field);

		std::vector<FieldData>* getFields() { return &m_fields; }
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

		bool insert(const std::string& table, const Rows& rows);

		bool insert(const std::string& table, const Row& row);

	private:
		std::string get_text_from_path(std::string path_file) const;

		std::string get_str_connection() const; /* returns string connection for library SOCI */

	};
}
