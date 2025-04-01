#pragma once
#include <optional>
#include <memory>
#include <string>

//Poco::Net::HTTPClientSession

namespace Poco {
	namespace Net {
		class HTTPClientSession;
	}
}

namespace yk {
	class Data;

	class FileGetHttpImpl {
	public:
		explicit FileGetHttpImpl(std::string url);
		~FileGetHttpImpl();

		std::shared_ptr<Data> Read(uint64_t length);

		std::optional<uint64_t> Size();

		bool Seek(int64_t offset);

		std::optional<bool> IsEnd();

		std::optional<bool> SeekAble();

		std::optional<int64_t> GetOffset();
	private:
		std::optional<bool> TestRangeSupport();
		std::shared_ptr<Poco::Net::HTTPClientSession> GenerateClientSession();
	private:
		std::string url_;
		std::string uri_path_;
		std::string uri_domain_;
		
		int64_t size_ = -1;       // �ļ��ܴ�С����ʼδ֪��
		int64_t position_ = 0;    // ��ǰ��ȡλ��
		int timeout_s_ = 12;      // ��ʱʱ�䣨Ĭ�� 6 �룩
	};


}
