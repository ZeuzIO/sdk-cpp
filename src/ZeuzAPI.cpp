#include "ZeuzAPI.h"

// CURL Helper
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size* nmemb);
	return size * nmemb;
}

ZeuzAPI::ZeuzAPI(std::string ZeuzAccessToken, std::string ZeuzAccessKey)
{
	_ZeuzAuth = new ZeuzAuth(ZeuzAccessToken, ZeuzAccessKey);
	IsDebugging = false;
	TimeOut = 20L;
}

ZeuzAPI::~ZeuzAPI()
{
	delete _ZeuzAuth;
}

CURLcode ZeuzAPI::APICall(EZeuzAPIMethod Method, std::string EndPoint, Json::Value &Request, Json::Value &Response)
{
	std::string Url(ZEUZ_API_URL + EndPoint);
	std::string signature = _ZeuzAuth->GenerateSignature(Request, Url);

	Json::StreamWriterBuilder JsonWriter;
	std::string JSONBody = Json::writeString(JsonWriter, Request);

	CURL *curl = curl_easy_init();
	if (!curl)
	{
		return CURLcode::CURLE_FAILED_INIT;
	}

	// Add headers
	struct curl_slist *Headers = NULL;
	std::stringstream BufferStream;
	BufferStream << "Authorization: " << "ZEUZ-HMAC-SHA512 " << _ZeuzAuth->GetAccessKey() << ":" << signature;
	Headers = curl_slist_append(Headers, BufferStream.str().c_str());
	BufferStream.str("");
	BufferStream << "X-Zeuz-Date: " << _ZeuzAuth->GetDate();
	Headers = curl_slist_append(Headers, BufferStream.str().c_str());
	BufferStream.str("");
	BufferStream << "Content-Type: " << "application/json";
	Headers = curl_slist_append(Headers, BufferStream.str().c_str());
	BufferStream.str("");
	if (Method != EZeuzAPIMethod::GET)
	{
		BufferStream << "Content-Length: " << JSONBody.length();
	}
	else {
		BufferStream << "Content-Length: " << 0;
	}
	Headers = curl_slist_append(Headers, BufferStream.str().c_str());
	BufferStream.str("");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, Headers);
	
	// Should we debug stuff here?
	if (IsDebugging)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	}

	curl_easy_setopt(curl, CURLOPT_TIMEOUT, TimeOut);
	curl_easy_setopt(curl, CURLOPT_URL, Url.c_str());

	if (Method == EZeuzAPIMethod::POST)
	{
		curl_easy_setopt(curl, CURLOPT_POST, 1);
	}

	if (Method == EZeuzAPIMethod::PUT)
	{
		curl_easy_setopt(curl, CURLOPT_PUT, 1);
	}

	if (Method != EZeuzAPIMethod::GET)
	{
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, JSONBody.c_str());
	}
	
	// Create Response Reader
	std::string _ResponseBuffer;
	std::string* _ResponseBufferPtr;
	_ResponseBufferPtr = std::addressof(_ResponseBuffer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, _ResponseBufferPtr);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	CURLcode ResponseCode = curl_easy_perform(curl);

	/* Check for errors */
	if (ResponseCode != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ResponseCode));
	}
	
	if(IsDebugging)
	{
		std::cout << "RESPONSE STR: " << _ResponseBuffer.c_str() << std::endl;
		std::cout << "ResponseCode: " << ResponseCode << std::endl;
	}

	curl_easy_cleanup(curl);
	curl_slist_free_all(Headers);
	Json::CharReaderBuilder JsonBuilder;
	Json::CharReader *JsonReader = JsonBuilder.newCharReader();
	std::string error;
	bool readingResult = JsonReader->parse(_ResponseBuffer.c_str(), _ResponseBuffer.c_str() + _ResponseBuffer.size(), &Response, &error);
	delete JsonReader;
	if (!readingResult)
	{
		std::cout << "[Zeuz] JSON Error:" << error << std::endl;
		return CURLcode::CURLE_READ_ERROR;
	}
	return ResponseCode;
}

CURLcode ZeuzAPI::ListServices(std::string ServerGroupId, std::string GameProfileId, Json::Value &OutServices)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/listServices/" + GameProfileId + "/" + ServerGroupId, Request, OutServices);
}

CURLcode ZeuzAPI::RequestService(std::string ServerGroupId, std::string GameProfileId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/provide/" + GameProfileId + "/" + ServerGroupId, Request, OutService);
}

CURLcode ZeuzAPI::ReserveService(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	Request["serviceId"] = ServiceId;
	return APICall(EZeuzAPIMethod::POST, "/service/reserve", Request, OutService);
}

CURLcode ZeuzAPI::UnreserveService(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	Request["serviceId"] = ServiceId;
	return APICall(EZeuzAPIMethod::POST, "/service/unreserve", Request, OutService);
}

CURLcode ZeuzAPI::GetStatus(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/status/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::GetService(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::AllocateService(std::string ServerGroupId, std::string GameProfileId, Json::Value &OutService)
{
	Json::Value Request;
	Request["gameProfileId"] = GameProfileId;
	Request["serverGroupId"] = ServerGroupId;
	return APICall(EZeuzAPIMethod::POST, "/service/allocate", Request, OutService);
}

CURLcode ZeuzAPI::UnallocateService(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/unallocate/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::Start(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/start/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::Stop(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/stop/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::Restart(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/restart/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::Reinstall(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/service/reinstall/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::PostStatistics(std::string ServiceId, int playerCount, int maxPlayerCount, Json::Value &OutService)
{
	Json::Value Request;
	Request["playerCount"] = playerCount;
	Request["maxPlayerCount"] = maxPlayerCount;
	return APICall(EZeuzAPIMethod::POST, "/statistics/ccu/" + ServiceId, Request, OutService);
}

CURLcode ZeuzAPI::GetStatistics(std::string ServiceId, Json::Value &OutService)
{
	Json::Value Request;
	return APICall(EZeuzAPIMethod::GET, "/statistics/ccu/" + ServiceId, Request, OutService);
}