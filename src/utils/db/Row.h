#pragma once

#include "FieldData.h"
#include "RegisterValue.h"

#include <vector>

namespace db
{
	/// <summary>
	/// Abstraction table row. 
	/// </summary>
	class Row
	{
	protected:
		Fields* m_lpFields; /* Pointer reference table fields. */
		std::vector<RegisterValue> m_registers; /* Registers of table. */

	public:

		/// <summary>
		/// Constructor.
		/// </summary>
		Row();

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="row">Reference row.</param>
		Row(const Row& row);

		/// <summary>
		/// Operator equals.
		/// </summary>
		/// <param name="row">Reference row.</param>
		/// <returns>Returns a reference to itself.</returns>
		Row& operator=(const Row& row);

		/// <summary>
		/// Adds a Register from a value.
		/// </summary>
		template<typename T>
		void addRegisterPerValue(const T& value)
		{
			RegisterValue registerValue;
			registerValue.set<T>(value);
			addRegister(registerValue);
		}

		/// <summary>
		/// Gets a value from register.
		/// </summary>
		template<typename T>
		T get(std::string field, T default = T()) const
		{
			int pos = find_field(field);
			if (pos < 0)
				return default;
			else
				return m_registers[pos].get<T>();
		}

		/// <summary>
		/// Adds a register.
		/// </summary>
		/// <param name="registerValue"></param>
		void addRegister(RegisterValue& registerValue);

		/// <summary>
		/// Sets information about fields.
		/// </summary>
		/// <param name="fields">Fields reference.</param>
		void setFieldData(Fields* fields);

		/// <summary>
		/// Finds register from field's name.
		/// </summary>
		/// <param name="fieldName">Field's name.</param>
		/// <returns>Returns position of register.</returns>
		int find_field(std::string fieldName) const;

		/// <summary>
		/// Gets all field names.
		/// </summary>
		/// <returns>Returns a string with all field names.</returns>
		std::string getSQLFieldNames() const;
		
		/// <summary>
		/// Gets all register of row.
		/// </summary>
		/// <returns>Returns a string with all register values.</returns>
		std::string getSQLRegisterValues() const;

		/// <summary>
		/// Gets a string for using it in SQL update query.
		/// </summary>
		/// <returns>Returns a string for making an update query with this object.</returns>
		std::string getSQLUpdateRegisterValues() const;
	};
}