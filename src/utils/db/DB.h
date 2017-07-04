#pragma once

#include "FieldData.h"
#include "RegisterValue.h"
#include "Rows.h"

#include <string>

namespace db
{
	/// <summary>
	/// This class allows send and receive queries to remote database.
	/// </summary>
	class DB
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
