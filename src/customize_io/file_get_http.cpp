#include "file_get_http.h"
#include <iostream>
#include "file.h"
#include "data.h"
#include "file_get_http_impl.h"

namespace yk
{

FileGetHttp::FileGetHttp(const std::string& path) : FileGetBase(path) {
	yk_file_ = std::make_shared<FileGetHttpImpl>(path);
}

std::optional<uint64_t> FileGetHttp::Size() {
	auto size_res = yk_file_->Size();
	if (!size_res.has_value()) {
		return {};
	}
	auto file_size = size_res.value();
	if (is_enc_file_) {
		if (file_size < kVideoHeaderLen_) {
			return {};
		}
		return { file_size - kVideoHeaderLen_ };
	}
	else {
        return { file_size };
	}
}

bool FileGetHttp::Seek(int64_t offset) {
	if (is_enc_file_) {
		auto enc_file_offset = offset + kVideoHeaderLen_;
		return yk_file_->Seek(enc_file_offset);
	}
	else {
		return yk_file_->Seek(offset);
	}
}

std::shared_ptr<Data> FileGetHttp::Read(uint64_t len) {

	{
		auto curr_offset_res = yk_file_->GetOffset();
		if (!curr_offset_res.has_value()) {
			return nullptr;
		}
		int64_t curr_offset = curr_offset_res.value();
		std::cout << "Read curr_offset: " << curr_offset << std::endl;
		if (is_enc_file_) {
			if (curr_offset < kVideoHeaderLen_) {
				yk_file_->Seek(kVideoHeaderLen_ + curr_offset);
			}
		}
	}


	{
		auto curr_offset_res = yk_file_->GetOffset();
		if (!curr_offset_res.has_value()) {
			return nullptr;
		}
		int64_t curr_offset = curr_offset_res.value();

		auto data_ptr = yk_file_->Read(len);
		if (!data_ptr) {
			return nullptr;
		}
		if (is_enc_file_) {
			DecryptData(data_ptr, curr_offset);
		}
		return data_ptr;
	}
}

std::optional<bool> FileGetHttp::IsEnd() {
	return yk_file_->IsEnd();
}

void FileGetHttp::Close() {
	return;
}

std::optional<bool> FileGetHttp::SeekAble() {
	return yk_file_->SeekAble();
}

}
