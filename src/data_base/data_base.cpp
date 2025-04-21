#include "data_base.h"
#include <iostream>
#include <sqlite3.h>
#include <sqlite_orm.h>
#include "cpp_base_lib/yk_logger.h"
#include "cpp_base_lib/time_ext.h"
namespace yk {

	using namespace sqlite_orm;

	sqlite3* db = nullptr;


	const std::string kDBDeviceNameKey = "devicename";

	const std::string kDBEncryKey = "encrykey";

	const std::string kDBName = "sdhaieu23s7dfk5698sd40485dhdfhg5mzqodhy298dspownz.db";


	auto MakeMediaListStorege() {
		return  make_storage(kDBName,
			make_table("media_list",
				make_column("id", &Database::MediaRecord::id, primary_key().autoincrement()),
				make_column("name", &Database::MediaRecord::name),
				make_column("url", &Database::MediaRecord::url),
				make_column("add_time", &Database::MediaRecord::add_time),
				make_column("last_play_time", &Database::MediaRecord::last_play_time)
			));
	}

	auto MakeKVStorege() {
		return  make_storage(kDBName,
			make_table("kv",
				make_column("id", &Database::KeyValue::id, primary_key().autoincrement()),
				make_column("key", &Database::KeyValue::key),
				make_column("value", &Database::KeyValue::value)
			));
	}


	bool ExistsColumn(sqlite3* db, const std::string& tableName, const std::string& columnName) {
		std::string query = "PRAGMA table_info(" + tableName + ")";
		sqlite3_stmt* stmt;
		int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
			YK_LOGE("{} Failed to prepare statement:", __func__, sqlite3_errmsg(db));
			return false;
		}
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			if (name && columnName == name) {
				sqlite3_finalize(stmt);
				return true; // 字段存在
			}
		}
		sqlite3_finalize(stmt);
		return false; // 字段不存在
	}

	void AddColumnIfNotExists(sqlite3* db, const std::string& tableName, const std::string& columnName, const std::string& columnType) {
		if (!ExistsColumn(db, tableName, columnName)) {
			std::string alterQuery = "ALTER TABLE " + tableName + " ADD COLUMN " + columnName + " " + columnType + ";";
			char* zErrMsg = nullptr;
			int rc = sqlite3_exec(db, alterQuery.c_str(), nullptr, 0, &zErrMsg);

			if (rc != SQLITE_OK) {
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				YK_LOGE("{} Column {} added error:{}", __func__, columnName, zErrMsg);
				sqlite3_free(zErrMsg);
			}
			else {
				fprintf(stdout, "Column '%s' added successfully\n", columnName.c_str());
				YK_LOGI("{} Column {} added successfully", __func__, columnName);
			}
		}
		else {
			fprintf(stdout, "Column '%s' already exists in table '%s'\n", columnName.c_str(), tableName.c_str());
			YK_LOGI("{} Column {} already exists", __func__, columnName);
		}
	}

	void PrintTableColumns(sqlite3* db, const char* tableName) {
		std::string query = "PRAGMA table_info(" + std::string(tableName) + ")";
		sqlite3_stmt* stmt;

		// 准备 SQL 语句
		int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
			YK_LOGE("{} Failed to prepare statement:", __func__, sqlite3_errmsg(db));
			return;
		}

		// 打印表头
		printf("Columns in table '%s':\n", tableName);
		printf("%-20s %-10s\n", "Column Name", "Type");

		// 遍历结果
		while (sqlite3_step(stmt) == SQLITE_ROW) {
			const char* columnName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
			const char* columnType = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
			printf("%-20s %-10s\n", columnName, columnType);
			YK_LOGI("columnName:{},  columnType: {}", columnName, columnType);
		}

		// 清理
		sqlite3_finalize(stmt);
	}


	/*static std::string DBName() {
		return db_name_default;
	}*/
	//展示方案
	const std::string db_display_scheme_key = "displayscheme";

	static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
		int i;
		for (i = 0; i < argc; i++) {
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		}
		printf("\n");
		return 0;
	}

	std::string create_media_list_table = R"(
		CREATE TABLE IF NOT EXISTS media_list (
			id INTEGER PRIMARY KEY AUTOINCREMENT, 
			name TEXT NOT NULL UNIQUE, 
			url TEXT NOT NULL,
			add_time INTEGER NOT NULL DEFAULT 0, 
			last_play_time INTEGER NOT NULL DEFAULT 0)
	)";

	std::string create_key_valuse_table = R"(
		CREATE TABLE IF NOT EXISTS kv (
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			key TEXT NOT NULL UNIQUE,
			value TEXT NOT NULL)
	)";


	


	

	Database::Database() {
		Init();
	}

	Database::~Database() {

	}

	void Database::Init() {
		char* zErrMsg = 0;
		int rc = 0;
		rc = sqlite3_open(kDBName.c_str(), &db);

		if (rc) {
			YK_LOGE("{} sqlite3_open error", __func__);
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
			return;
		}
		else {
			YK_LOGI("{} Opened database successfully", __func__);
			fprintf(stderr, "Opened database successfully\n");
		}

		rc = sqlite3_exec(db, create_media_list_table.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			YK_LOGE("{} create_users_table SQL error is {}", __func__, zErrMsg);
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Table created successfully\n");
		}

		rc = sqlite3_exec(db, create_key_valuse_table.c_str(), callback, 0, &zErrMsg);
		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			YK_LOGE("{} create_key_valuse_table SQL error is {}", __func__, zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else {
			fprintf(stdout, "Table kv created successfully\n");
		}

		// 追加字段
		//AddColumnIfNotExists(db, "material", "tencent_auditing_status", "integer");
		//AddColumnIfNotExists(db, "material", "tencent_auditing_desc", "text");
		//printTableColumns(db, "material");
	}

	std::tuple<bool, std::string> Database::UpdateDeviceName(const std::string& devname) {
		try {
			auto storage = MakeKVStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			auto ids = storage.select(&KeyValue::id, where(c(&KeyValue::key) == kDBDeviceNameKey));
			std::cout << "ids size = " << ids.size() << std::endl;
			if (!ids.empty()) {
				auto id = ids[0];
				auto dev_kv = storage.get<KeyValue>(id);
				dev_kv.value = devname;
				storage.update(dev_kv);
				return { true, "" };
			}
			KeyValue new_kv{ -1, kDBDeviceNameKey, devname };
			int id = storage.insert(new_kv);
			std::cout << "insert Devicename id = " << id << std::endl;
			return { id > 0, "" };
		}
		catch (std::exception& e) {
			std::cout << "UpdateDevicename error " << e.what() << std::endl;
			return { false, std::string(e.what()) };
		}
	}

	std::tuple<bool, std::string, std::string> Database::GetDeviceName() {
		try {
			auto storage = MakeKVStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			auto ids = storage.select(&KeyValue::id, where(c(&KeyValue::key) == kDBDeviceNameKey));
			std::cout << "ids size = " << ids.size() << std::endl;
			if (!ids.empty()) {
				auto id = ids[0];
				auto dev_kv = storage.get<KeyValue>(id);
				return { true, dev_kv.value, "" };
			}
			return { false, "", "device_name not set" };
		}
		catch (std::exception& e) {
			std::cout << "get device_name error " << e.what() << std::endl;
			return { false, "", std::string(e.what()) };
		}
	}

	std::tuple<bool, std::string> Database::SetKey(const std::string& value) {
		try {
			auto storage = MakeKVStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			auto ids = storage.select(&KeyValue::id, where(c(&KeyValue::key) == kDBEncryKey));
			std::cout << "ids size = " << ids.size() << std::endl;
			if (!ids.empty()) {
				auto id = ids[0];
				auto encrykey_kv = storage.get<KeyValue>(id);
				encrykey_kv.value = value;
				storage.update(encrykey_kv);
				return { true, "" };
			}
			KeyValue new_kv{ -1, kDBEncryKey, value };
			int id = storage.insert(new_kv);
			std::cout << "insert key id = " << id << std::endl;
			return { id > 0, "" };
		}
		catch (std::exception& e) {
			std::cout << "set key error " << e.what() << std::endl;
			return { false, std::string(e.what()) };
		}
	}

	std::tuple<bool, std::string, std::string> Database::GetKey() {
		try {
			auto storage = MakeKVStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			auto ids = storage.select(&KeyValue::id, where(c(&KeyValue::key) == kDBEncryKey));
			std::cout << "ids size = " << ids.size() << std::endl;
			if (!ids.empty()) {
				auto id = ids[0];
				auto encrykey_kv = storage.get<KeyValue>(id);
				return { true, encrykey_kv.value, "" };
			}
			return { false, "", "key not set" };
		}
		catch (std::exception& e) {
			std::cout << "get key error " << e.what() << std::endl;
			return { false, "", std::string(e.what()) };
		}
	}

	std::tuple<bool, std::string> Database::InsertMediaRecord(const std::string& name, const std::string& url) {
		try {
			auto storage = MakeMediaListStorege();
			auto ids = storage.select(&MediaRecord::id, where(c(&MediaRecord::name) == name and (c(&MediaRecord::url) == url)));
			std::cout << "ids size = " << ids.size() << std::endl;
			if (!ids.empty()) {
				return { false, "Media Record exists" };
			}
			MediaRecord new_record{ -1, name, url };
			new_record.add_time = new_record.last_play_time = TimeExt::GetCurrentTimestamp() / 1000;
			std::lock_guard<std::mutex> lck{ mutex_ };
			int id = storage.insert(new_record);
			std::cout << "InsertMediaRecord id = " << id << std::endl;
			return { id > 0, "" };
		}
		catch (std::exception& e) {
			std::cout << "InsertMediaRecord error " << e.what() << std::endl;
			return { false, std::string(e.what()) };
		}
	}


	std::tuple<bool, std::string> Database::ExistMediaRecord(const std::string& name, const std::string& url) {
		try {
			auto storage = MakeMediaListStorege();
			auto ids = storage.select(&MediaRecord::id, where(c(&MediaRecord::name) == name and (c(&MediaRecord::url) == url)));
			return { !ids.empty(), "" };
		}
		catch (std::exception& e) {
			return { false, std::string(e.what()) };
		}
	}

	std::tuple<bool, std::string> Database::UpdateMediaRecord(const std::string& name, const std::string& url) {
		try {
			auto storage = MakeMediaListStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			auto ids = storage.select(&MediaRecord::id, where(c(&MediaRecord::name) == name and (c(&MediaRecord::url) == url)));
			if (ids.empty()) {
				return { false, "The MediaRecord does not exist" };
			}
			auto id = ids[0];
			auto record = storage.get<MediaRecord>(id);
			record.last_play_time = TimeExt::GetCurrentTimestamp() / 1000;
			return { true, "" };
		}
		catch (std::exception& e) {
			return { false, std::string(e.what()) };
		}
	}


	std::tuple<bool, std::string> Database::DeleteMediaRecord(int id) {
		try {
			auto storage = MakeMediaListStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			storage.remove_all<MediaRecord>(where(c(&MediaRecord::id) == id));
			return { true, "" };
		}
		catch (std::exception& e) {
			return { false, std::string(e.what()) };
		}
	}

	std::tuple<bool, std::string, std::vector<Database::MediaRecord>> Database::GetMediaRecordList() {
		try {
			auto storage = MakeMediaListStorege();
			std::lock_guard<std::mutex> lck{ mutex_ };
			auto results = storage.get_all<Database::MediaRecord>();
			if (results.empty()) {
				return { false, "The MediaRecordList is null", {} };
			}
			return { true, "", results };
		}
		catch (std::exception& e) {
			return { false, std::string(e.what()), {} };
		}
	}
}
