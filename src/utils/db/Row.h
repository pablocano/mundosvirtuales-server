#pragma once

#include "FieldData.h"
#include "RegisterValue.h"

#include <vector>
#include <memory>

namespace db
{
	/// <summary>
	/// Abstraction table row.
	/// </summary>
	class Row
	{
	protected:
		std::shared_ptr<Fields> m_lpFields; /* Pointer reference table fields. */
		Registers m_registers; /* Registers of table. */

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

		const std::shared_ptr<Fields> getFields() const
		{
			return m_lpFields;
		}

		const Registers& getRegisters() const
		{
			return m_registers;
		}

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
			if (pos < 0 || m_registers.size() <= 0)
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
		void setFieldData(std::shared_ptr<Fields> fields);

		/// <summary>
		/// Finds register from field's name.
		/// </summary>
		/// <param name="fieldName">Field's name.</param>
		/// <returns>Returns position of register.</returns>
		int find_field(std::string fieldName) const;

		bool isEmpty() const;
	};
}