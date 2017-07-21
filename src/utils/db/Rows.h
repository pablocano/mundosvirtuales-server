#pragma once

#include "Row.h"
#include "FieldData.h"

#include <vector>

namespace db
{
	class Rows
	{
	protected:
		std::shared_ptr<Fields> m_fields;
		std::vector<Row> m_rows;

		void setFields();

	public:
		Rows();

		Rows(const Rows& rows);

		Rows& operator=(const Rows& rows);

		void push_back(Row& row);

		Row& front();

		Row& back();

		std::vector<Row>::iterator begin();

		std::vector<Row>::iterator end();

		std::vector<Row>::const_iterator cbegin() const;

		std::vector<Row>::const_iterator cend() const;

		int size() const;

		bool isEmpty() const;

		void addField(FieldData field);

		std::shared_ptr<Fields> getFields();
	};
}