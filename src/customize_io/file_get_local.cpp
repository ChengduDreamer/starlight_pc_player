#include "file_get_local.h"
#include <algorithm>
#include <iostream>
#include "file.h"
#include "data.h"
//#include "decryptor.h"
//#include "StreamingCryptor.h"
//#include "cert/RC4.h"
namespace yk
{
	FileGetLocal::FileGetLocal(const std::string& path) : FileGetBase(path) {
		dl_file_ = yk::File::OpenForReadB(path_);
	}

	std::optional<uint64_t> FileGetLocal::Size() {
		auto size_res = dl_file_->Size();
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

	bool FileGetLocal::Seek(int64_t offset) {
		if (is_enc_file_) {
			auto enc_file_offset = offset + kVideoHeaderLen_;
			return dl_file_->Seek(enc_file_offset);
		}
		else {
			return dl_file_->Seek(offset);
		}
	}

	std::shared_ptr<Data> FileGetLocal::Read(uint64_t len) {
		if (!dl_file_) {
			return nullptr;
		}
		{
			auto curr_offset_res = dl_file_->GetOffset();
			if (!curr_offset_res.has_value()) {
				return nullptr;
			}
			int64_t curr_offset = curr_offset_res.value();
			std::cout << "Read curr_offset: " << curr_offset << std::endl;
			if (is_enc_file_) {
				if (curr_offset < kVideoHeaderLen_) {
					dl_file_->Seek(kVideoHeaderLen_ + curr_offset);
				}
			}
		}

		{
			auto curr_offset_res = dl_file_->GetOffset();
			if (!curr_offset_res.has_value()) {
				return nullptr;
			}
			int64_t curr_offset = curr_offset_res.value();
			auto data_ptr = dl_file_->Read(len);
			if (!data_ptr) {
				return nullptr;
			}
			int64_t pos = curr_offset - kVideoHeaderLen_;
			std::cout << "pos: " << pos << std::endl;
			if (is_enc_file_) {
				DecryptData(data_ptr, curr_offset);
			}
			return data_ptr;
		}
	}

	std::optional<bool> FileGetLocal::IsEnd() {
        if (!dl_file_) {
            return {};
        }
		return dl_file_->IsEnd();
	}

	void FileGetLocal::Close() {
		if (!dl_file_) {
			return;;
		}
		dl_file_->Close();
	}

	std::optional<bool> FileGetLocal::SeekAble() {
		return {true};
	}
}
