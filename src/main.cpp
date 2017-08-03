#include "utils/db/DBAdapterSOCI.h"
#include "utils/platform/SystemCall.h"
#include "utils/logger/Logger.h"
#include "utils/util.h"
#include "ServerPlant.h"

#include <iostream>
#include <string>

static DBAdapterSOCI dataBase("test_proter", "postgres", "127.0.0.1", 5432, "test123");

void observerLogDataBase(std::string type, std::chrono::system_clock::time_point date, std::string source, std::string message)
{
	using namespace db;

	Row row;
	std::shared_ptr<Fields> fieldData(new Fields());

	fieldData->push_back(FieldData("users_id", TypeData::DB_INTEGER));
	fieldData->push_back(FieldData("type_log", TypeData::DB_STRING));
	fieldData->push_back(FieldData("source", TypeData::DB_STRING));
	fieldData->push_back(FieldData("message", TypeData::DB_STRING));
	fieldData->push_back(FieldData("created_at", TypeData::DB_DATE));

	row.setFieldData(fieldData);

	row.addRegisterPerValue<int>(1); // Root user.
	row.addRegisterPerValue<std::string>(type);
	row.addRegisterPerValue<std::string>(source);
	row.addRegisterPerValue<std::string>(message);
	row.addRegisterPerValue<std::tm>(to_calendar_time(date));

	dataBase.insert("Logger", row);
}

int main()
{
	ServerPlant server(&dataBase);

	// LOGGER_ADD_OBSERVER(observerLogDataBase); // TODO: first to use, it is necessary to fix mistake

	LOGGER_LOG("Server", "Starting Server");

	server.start();

	SystemCall::sleep(8 * 60 * 60 * 1000);

	server.stop(); // wait until server ends last connection

	LOGGER_LOG("Server", "Press enter to finish");

	std::getchar();

	return 0;
}