#pragma once
#include <memory>
#include "file_get_base.h"

namespace yk
{
class FileGetHttpImpl;

class FileGetHttp : public FileGetBase
{
public:
	FileGetHttp(const std::string& path);
	virtual std::optional<uint64_t> Size() override;
	virtual bool Seek(int64_t offset) override;
	virtual std::shared_ptr<Data> Read(uint64_t len) override;
	virtual std::optional<bool> IsEnd() override;
	virtual void Close() override;
	virtual std::optional<bool> SeekAble() override;
private:
	std::shared_ptr<FileGetHttpImpl> yk_file_ = nullptr;
};

}
