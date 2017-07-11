#pragma once

#include "FieldData.h"
#include "RegisterValue.h"
#include "Rows.h"

#include <string>

namespace db
{
	/// <summary>
	/// Abstract class allows send and receive queries to remote database.
	/// </summary>
	class DBAdapter
	{
	protected:
		std::string db_user; /* User database. */
		std::string db_password; /* Password database. */
		std::string db_engine; /* Engine database. */
		std::string db_name; /* Name database. */
		std::string db_host; /* Host database. */
		int db_port; /* Port database. */

		int current_language_id; /* Current language. */

	public:
		DBAdapter(std::string _db_name, std::string _db_user, std::string _db_host = "", int _db_port = 5432, std::string _db_password = "", std::string _db_engine = "postgresql");

		void set_language(std::string lang);

		virtual Rows query(std::string query) const = 0;

		virtual int countQuery(std::string table, std::string where) const = 0;

		virtual bool insert(const std::string& table, const Rows& rows) = 0;

		virtual bool insert(const std::string& table, const Row& row) = 0;

		virtual bool update(const std::string& table, const Rows& rows, const std::string name_id) = 0;

		virtual bool update(const std::string& table, const Row& row, const std::string where) = 0;

	protected:
		std::string get_text_from_path(std::string path_file) const;

		std::string get_str_connection() const; /* returns string connection for library SOCI */

	};
}
