#include "file_get_http_impl.h"
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>
#include <iostream>
#include "data.h"

namespace yk {

	FileGetHttpImpl::FileGetHttpImpl(std::string url) {
		url_ = url;

	}

	FileGetHttpImpl::~FileGetHttpImpl() {

	}

	std::shared_ptr<Data> FileGetHttpImpl::Read(uint64_t length) {
        try {
            if (length <= 0) {
                return nullptr;
            }
            std::shared_ptr<Poco::Net::HTTPClientSession> client_session = GenerateClientSession();
            if (!client_session) {
                std::cout << "GenerateClientSession is nullptr" << std::endl;
                return nullptr;
            }
            std::string range_value = std::string("bytes=") + std::to_string(position_) + std::string("-") + std::to_string(position_ + length - 1);
            Poco::URI uri(url_);
            std::string path = uri.getPathAndQuery();
            if (path.empty()) {
                path = "/";
            }
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path);
            request.set("Range", range_value);


            std::cout << "FileGetHttpImpl Read, range_value: " << range_value << std::endl;


            client_session->sendRequest(request);
            Poco::Net::HTTPResponse response;
            std::istream& response_stream = client_session->receiveResponse(response);
            std::string output_data;
            if (Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT != response.getStatus() && Poco::Net::HTTPResponse::HTTP_OK != response.getStatus()) {
                std::cout <<"status:" << response.getStatus() << "reson:" << response.getReason() << std::endl;
                return nullptr;
            }
            Poco::StreamCopier::copyToString(response_stream, output_data);
            if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT || (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK && response.getContentLength() == length)) {
                DataPtr data = Data::Make(output_data.c_str(), output_data.size());
                position_ = position_ + data->Size();
                return data;
            }
        }
        catch (const Poco::Exception& e) {
            std::cerr << "Error: " << e.displayText() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Standard Exception: " << e.what() << std::endl;
            return {};
        }
        catch (...) {
            std::cerr << "Unknown error occurred." << std::endl;
            return {};
        }
		return nullptr;
	}

	bool FileGetHttpImpl::Seek(int64_t offset) {
		position_ = offset;
		return true;
	}

	std::optional<uint64_t> FileGetHttpImpl::Size() {
        try {
            std::shared_ptr<Poco::Net::HTTPClientSession> client_session = GenerateClientSession();
            if (!client_session) {
                std::cout << "GenerateClientSession is nullptr" << std::endl;
                return {};
            }

            Poco::URI uri(url_);
            std::string path = uri.getPathAndQuery();
            if (path.empty()) {
                path = "/";
            }
            std::cout << "path: " << path << std::endl;

            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_HEAD, path);
            client_session->sendRequest(request);
            Poco::Net::HTTPResponse response;
            std::istream& rs = client_session->receiveResponse(response);
            if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK ||
                response.getStatus() == Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT) {
                if (response.hasContentLength()) {
                    size_ = response.getContentLength();
                    return { size_ };
                }
                else {
                    std::cout << "not found ContentLength" << std::endl;
                }
            }
            std::cout << "status:" << response.getStatus() << "reson:" << response.getReason() << std::endl;
            return {};
        }
        catch (const Poco::Exception& e) {
            std::cerr << "Error: " << e.displayText() << std::endl;
            return {};
        }
        catch (const std::exception& e) {
            std::cerr << "Standard Exception: " << e.what() << std::endl;
            return {};
        }
        catch (...) {
            std::cerr << "Unknown error occurred." << std::endl;
            return {};
        }
	}

	std::optional<bool> FileGetHttpImpl::TestRangeSupport() {
        try {
            std::shared_ptr<Poco::Net::HTTPClientSession> client_session = GenerateClientSession();
            if (!client_session) {
                std::cout << "GenerateClientSession is nullptr" << std::endl;
                return {};
            }

            Poco::URI uri(url_);
            std::string path = uri.getPathAndQuery();
            if (path.empty()) { 
                path = "/"; 
            }

            Poco::Net::HTTPRequest range_request(Poco::Net::HTTPRequest::HTTP_GET, path);
            range_request.set("Range", "bytes=0-0"); // 请求第一个字节

            client_session->sendRequest(range_request);
            Poco::Net::HTTPResponse range_response;
            client_session->receiveResponse(range_response);

            if (range_response.getStatus() == Poco::Net::HTTPResponse::HTTP_PARTIAL_CONTENT) {
                return {true}; // 服务器支持 Range 请求
            }
            return {false};
        }
        catch (const Poco::Exception& e) {
            std::cerr << "Error: " << e.displayText() << std::endl;
            return {};
        }
        catch (const std::exception& e) {
            std::cerr << "Standard Exception: " << e.what() << std::endl;
            return {};
        }
        catch (...) {
            std::cerr << "Unknown error occurred." << std::endl;
            return {};
        }
	}

	std::optional<bool> FileGetHttpImpl::IsEnd() {
        if (-1 == size_) {
            return {};
        }
        return { position_ >= size_ };
	}

	std::optional<bool> FileGetHttpImpl::SeekAble() {
		return TestRangeSupport();
	}

	std::optional<int64_t> FileGetHttpImpl::GetOffset() {
		return { position_ };
	}

    std::shared_ptr<Poco::Net::HTTPClientSession> FileGetHttpImpl::GenerateClientSession() {
        Poco::URI uri(url_);
        std::shared_ptr<Poco::Net::HTTPClientSession> client_session = nullptr;
        if (uri.getScheme() == "https") {
            client_session = std::make_shared<Poco::Net::HTTPSClientSession>(uri.getHost(), uri.getPort());
        }
        else {
            client_session = std::make_shared<Poco::Net::HTTPClientSession>(uri.getHost(), uri.getPort());
        }
        std::cout << "uri.getScheme(): " << uri.getScheme() << "; host: "<< uri.getHost() << "; port: " << uri.getPort() << std::endl;
        client_session->setKeepAlive(false);
        client_session->setTimeout(Poco::Timespan(12, 0));
        return client_session;
    }
}