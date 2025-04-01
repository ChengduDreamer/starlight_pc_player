#pragma once
#include <memory>
#include "file_get_base.h"

namespace yk
{

class File;
class FileGetLocal : public FileGetBase
{
public:
	FileGetLocal(const std::string& path);
	virtual std::optional<uint64_t> Size() override;
	virtual bool Seek(int64_t offset) override;
	virtual std::shared_ptr<Data> Read(uint64_t len) override;
	virtual std::optional<bool> IsEnd() override;
	virtual void Close() override;
	virtual std::optional<bool> SeekAble() override;
private:
	std::shared_ptr<yk::File> dl_file_ = nullptr;
};

}
