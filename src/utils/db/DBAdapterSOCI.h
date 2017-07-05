#pragma once

#include "DBAdapter.h"
#include "FieldData.h"
#include "RegisterValue.h"
#include "Rows.h"

#include <string>

namespace db
{
	/// <summary>
	/// This class allows send and receive queries to remote database.
	/// </summary>
	class DBAdapterSOCI :  public DBAdapter
	{
	public:
		DBAdapterSOCI(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");

		Rows query(std::string query) const;

		int countQuery(std::string table, std::string where) const;

		bool insert(const std::string& table, const Rows& rows);

		bool insert(const std::string& table, const Row& row);
	};
}
