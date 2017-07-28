#pragma once

#include "FieldData.h"
#include "RegisterValue.h"
#include "Rows.h"

#include <string>
#include <sstream>
#include <algorithm>

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

		virtual void onlyQuery(std::string query) const = 0;

		virtual Rows query(std::string query) const = 0;

		virtual int countQuery(std::string table, std::string where) const = 0;

		virtual std::vector<int> insert(const std::string& table, const Rows& rows) = 0;

		virtual int insert(const std::string& table, const Row& row) = 0;

		virtual bool update(const std::string& table, const Rows& rows, const std::string name_id) = 0;

		virtual bool update(const std::string& table, const Row& row, const std::string where) = 0;

	protected:
		std::string get_text_from_path(std::string path_file) const;

		std::string get_str_connection() const; /* returns string connection for library SOCI */

	protected:

		static std::string getSQLFieldNames(const Row& row, std::vector<int>& skip_pos)
		{
			std::shared_ptr<Fields> lpFields = row.getFields();
			std::stringstream ss;
			int n = (int) lpFields->size();

			ss.str("");
			ss << "(";

			for (int pos = 0; pos < n; ++pos)
			{
				if (std::find(skip_pos.begin(), skip_pos.end(), pos) == skip_pos.end())
				{
					ss << lpFields->at(pos).getName();
					
					if (pos < n - 1)
						ss << ", ";
				}
			}

			ss << ")";

			return ss.str();
		}

		static std::string getSQLFieldNames(const Row& row)
		{
			std::shared_ptr<Fields> lpFields = row.getFields();
			std::stringstream ss;
			int n = (int) lpFields->size();

			ss.str("");
			ss << "(";

			for (int pos = 0; pos < n; ++pos)
			{
				ss << lpFields->at(pos).getName();

				if (pos < n - 1)
					ss << ", ";
			}

			ss << ")";

			return ss.str();
		}

		static std::string getSQLRegisterValues(const Row& row, std::vector<int>& skip_pos)
		{
			const Registers& registers = row.getRegisters();
			std::stringstream ss;
			int n = (int) registers.size();

			ss.str("");
			ss << "(";
			
			for (int pos = 0; pos < n; ++pos)
			{
				if (std::find(skip_pos.begin(), skip_pos.end(), pos) == skip_pos.end())
				{
					ss << registers.at(pos).getSQLValue();

					if (pos < n - 1)
						ss << ", ";
				}
			}

			ss << ")";

			return ss.str();
		}

		static std::string getSQLRegisterValues(const Row& row)
		{
			const Registers& registers = row.getRegisters();
			std::stringstream ss;
			int n = (int) registers.size();

			ss.str("");
			ss << "(";

			for (int pos = 0; pos < n; ++pos)
			{
				ss << registers.at(pos).getSQLValue();

				if (pos < n - 1)
					ss << ", ";
			}

			ss << ")";

			return ss.str();
		}

		static std::string getSQLUpdateRegisterValues(const Row& row)
		{
			std::shared_ptr<Fields> lpFields = row.getFields();
			const Registers& registers = row.getRegisters();
			std::stringstream ss;
			int n = (int) registers.size();

			ss.str("");

			for (int pos = 0; pos < n; ++pos)
			{
				ss << lpFields->at(pos).getName() << " = " << registers.at(pos).getSQLValue();
				
				if (pos < n - 1)
					ss << ", ";
			}

			return ss.str();
		}

		static int getPos(std::shared_ptr<Fields> lpFields, std::string s)
		{
			int pos = (int)std::distance(lpFields->begin(), std::find(lpFields->begin(), lpFields->end(), s));
			return (pos < (int) lpFields->size() ? pos : -1);
		}
	};
}
