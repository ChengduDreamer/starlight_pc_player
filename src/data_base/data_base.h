#pragma once

#include <tuple>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

namespace yk {
	
	class MediaRecord {
	public:
		static std::shared_ptr<MediaRecord> Make(const std::string& name, const std::string& url) {
			auto ptr = std::make_shared<MediaRecord>();
			ptr->name = name;
			ptr->url = url;
			return ptr;
		}

		int id = -1;
		uint64_t last_pos = 0;
		std::string name;
		std::string url;
		int64_t add_time = 0;
		int64_t last_play_time = 0;
	};

	class Database {
	public:
		
		Database();
		~Database();
		void Init();
		static Database* Instance() {
			static Database self;
			return &self;
		}

		struct KeyValue {
			int id;
			std::string key;
			std::string value;
		};

	
		std::tuple<bool, std::string> SetKey(const std::string& key);
		std::tuple<bool, std::string, std::string> GetKey();
		std::tuple<bool, std::string> UpdateDeviceName(const std::string& devname);
		std::tuple<bool, std::string, std::string> GetDeviceName();

		std::tuple<bool, std::string> InsertMediaRecord(std::shared_ptr<MediaRecord> record_ptr);
		std::tuple<bool, std::string> InsertMediaRecord(const std::string& name, const std::string& url);
		std::tuple<bool, std::string> ExistMediaRecord(const std::string& name, const std::string& url);
		std::tuple<bool, std::string> UpdateMediaRecord(const std::string& name, const std::string& url);
		std::tuple<bool, std::string> DeleteMediaRecord(int id);

		// to do: 以后考虑分页
		std::tuple<bool, std::string, std::vector<MediaRecord>> GetMediaRecordList();
	private:
		std::mutex mutex_;

		
	};

}