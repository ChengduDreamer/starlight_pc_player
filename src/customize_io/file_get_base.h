#pragma once
#include <string>
#include <memory>
#include <optional>

namespace yk
{
class Data;
class Decryptor;
class CStreamCryptor;

class FileGetBase
{
public:
	FileGetBase(const std::string& path);
	
	virtual std::optional<uint64_t> Size() = 0;
	virtual bool Seek(int64_t offset) = 0;
	virtual std::shared_ptr<Data> Read(uint64_t len) = 0;
	virtual std::optional<bool> IsEnd() = 0;
	virtual void Close() = 0;
	virtual std::optional<bool> SeekAble() = 0;
	virtual void InitDecryptor();
	
	void DecryptData(std::shared_ptr<Data> data_ptr, uint64_t curr_offset);

	std::string path_;
	const uint32_t kVideoHeaderLen_ = 4096;

	bool streamenc_enable_ = false;
	std::shared_ptr<Decryptor> decryptor_ = nullptr;
	std::shared_ptr<CStreamCryptor> dolit_streamxor_ = nullptr;
	int stream_cryptor_version_ = 0;

	bool is_enc_file_ = false;
};

}
