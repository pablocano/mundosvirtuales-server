#pragma once

#include "Row.h"
#include "FieldData.h"

#include <vector>

namespace db
{
	class Rows
	{
	protected:
		std::vector<FieldData> m_fields;
		std::vector<Row> m_rows;

		void setFields();

	public:
		Rows();

		Rows(const Rows& rows);

		Rows& operator=(const Rows& rows);

		void push_back(Row& row);

		Row& back();

		std::vector<Row>::iterator begin();

		std::vector<Row>::iterator end();

		std::vector<Row>::const_iterator cbegin() const;

		std::vector<Row>::const_iterator cend() const { return m_rows.cend(); }

		void addField(FieldData field);

		std::vector<FieldData>* getFields();
	};
}