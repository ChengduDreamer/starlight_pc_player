#include "file_get_base.h"
#include <string>
#include "data.h"
//#include "decryptor.h"
//#include "StreamingCryptor.h"
namespace yk {
	FileGetBase::FileGetBase(const std::string& path) : path_(path) {
		//decryptor_ = Decryptor::Make(path_);
	}

	void FileGetBase::InitDecryptor() {
#if 0
		decryptor_->InitDecryKey();
		streamenc_enable_ = decryptor_->GetStreamencEnable();
		if (streamenc_enable_) {
			dolit_streamxor_ = decryptor_->GetStreamCryptor();
			stream_cryptor_version_ = decryptor_->GetStreamCryptorVersion();
		} else {
			crc4_ = decryptor_->GetCrc4Cryptor();
		}
#endif
	}

	void FileGetBase::DecryptData(std::shared_ptr<Data> data_ptr, uint64_t curr_offset) {
#if 0
		if (streamenc_enable_ && dolit_streamxor_) {
			dolit_streamxor_->porocess(curr_offset - kDolitVideoHeaderLen_, (uint8_t*)data_ptr->DataAddr(), (uint8_t*)data_ptr->DataAddr(), data_ptr->Size(), stream_cryptor_version_);
		}
#endif
	}
}