#pragma once

#include "FieldData.h"
#include "../logger/Logger.h"

#include <memory>

namespace db
{
	/// <summary>
	/// Register status.
	/// </summary>
	enum IndicatorField
	{
		IS_OK, /* No problem. */
		IS_NULL, /* Null value. */
		IS_TRUNCATED /* Trucated value. */
	};

	/// <summary>
	/// Storages a value (any data type), this value represent a register in database.
	/// </summary>
	class RegisterValue
	{
	protected:
		void* m_value; /* Pointer that reference to register value. */
		int m_size; /* Size of value. */
		IndicatorField m_indicator; /* Status of register. */
		FieldData* m_fieldData; /* Information about field. */

	public:

		/// <summary>
		/// Constructor.
		/// </summary>
		RegisterValue();

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="w">Reference register value.</param>
		RegisterValue(const RegisterValue& w);

		/// <summary>
		/// Move contructor.
		/// </summary>
		/// <param name="w">Reference register value.</param>
		RegisterValue(RegisterValue&& w);

		/// <summary>
		/// Operator equals.
		/// </summary>
		/// <param name="w">Reference register value.</param>
		/// <returns>Returns a reference to itself.</returns>
		RegisterValue& operator=(const RegisterValue& w);

		/// <summary>
		/// Destructor.
		/// </summary>
		~RegisterValue();

		/// <summary>
		/// Sets value as to storage.
		/// </summary>
		template<typename T>
		void set(const T& p);

		/// <summary>
		/// Sets value (string) as to storage.
		/// </summary>
		template<>
		void set<std::string>(const std::string& p);

		/// <summary>
		/// Gets value.
		/// </summary>
		template<typename T>
		T get() const;

		/// <summary>
		/// Gets value (string).
		/// </summary>
		template<>
		std::string get<std::string>() const;

		/// <summary>
		/// Gets indicator of register.
		/// </summary>
		/// <returns></returns>
		IndicatorField getIndicator() const;

		/// <summary>
		/// Sets indicator of register.
		/// </summary>
		/// <param name="indicator"></param>
		void setIndicator(IndicatorField indicator);

		/// <summary>
		/// Gets field informartion.
		/// </summary>
		/// <returns></returns>
		FieldData getFieldData();

		/// <summary>
		/// Sets field information.
		/// </summary>
		/// <param name="field">Information field.</param>
		void setFieldData(FieldData* field);

		/// <summary>
		/// Gets a string with stored value.
		/// </summary>
		/// <returns>Returns string with stored value.</returns>
		std::string getSQLValue() const;
	};

	template<typename T>
	inline void RegisterValue::set(const T & p)
	{
		m_size = sizeof(T);
		if (m_value)
			m_value = realloc(m_value, m_size);
		else
			m_value = malloc(m_size);

		if (m_value)
			memcpy(m_value, static_cast<const void *>(&p), m_size);
		else
			LOGGER_DEBUG("RegisterValue", "It doesn't work allocation memory for register.");
	}

	template<>
	inline void RegisterValue::set(const std::string & p)
	{
		m_size = (int)p.length() + 1;
		if (m_value)
			m_value = realloc(m_value, m_size);
		else
			m_value = malloc(m_size);

		if (m_value)
		{
			memcpy(m_value, static_cast<const void *>(p.data()), p.length());
			*((char *)m_value + p.length()) = '\0';
		}
		else
			LOGGER_DEBUG("RegisterValue", "It doesn't work allocation memory for register.");
	}

	template<typename T>
	inline T RegisterValue::get() const
	{
		T d = *static_cast<T*>(m_value);
		return d;
	}

	template<>
	inline std::string RegisterValue::get() const
	{
		char* lpS = static_cast<char *>(m_value);
		std::string s = std::string(lpS);
		return s;
	}
}