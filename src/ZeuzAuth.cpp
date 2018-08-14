#include "ZeuzAuth.h"


ZeuzAuth::ZeuzAuth(std::string ZeuzAccessToken, std::string ZeuzAccessKey)
	: _AccessToken(ZeuzAccessToken), _AccessKey(ZeuzAccessKey)
{
}

ZeuzAuth::~ZeuzAuth()
{
}

void ZeuzAuth::GetRequestParameters(Json::Value &Requestbody, std::vector<std::string> &OutParameters)
{
	for (Json::Value::const_iterator It1 = Requestbody.begin(); It1 != Requestbody.end(); ++It1)
	{
		Json::Value RequestSubRoot = Requestbody[It1.key().asString()];
		if (RequestSubRoot.type() == Json::objectValue || RequestSubRoot.type() == Json::arrayValue)
		{
			GetRequestParameters(RequestSubRoot, OutParameters);
		}
		else
		{
			OutParameters.push_back(RequestSubRoot.asString());
		}
	}
}

std::string ZeuzAuth::GenerateSignature(Json::Value &RequestBody, std::string Url)
{

	std::vector<std::string> Parameters;
	GetRequestParameters(RequestBody, Parameters);
	std::string StringToSign = GetStringToSign(Parameters, Url);

	std::ostringstream BufferStream;
	BufferStream << GetAccessKey() << "\n";

	BufferStream << ZEUZAUTH_ALGORITHM_SHA512 << "\n";

	BufferStream << GetDate() << "\n";
	BufferStream << StringToSign;

	std::string Buffer = BufferStream.str();

	unsigned char* InputBuffer = new unsigned char[Buffer.length() + 1];
	std::strcpy((char*)InputBuffer, Buffer.c_str());

	unsigned char OutputBuffer[EVP_MAX_MD_SIZE];
	unsigned int OutputSize;
	
	
	HMAC(EVP_sha512(), GetAccessToken().c_str(), GetAccessToken().size(), InputBuffer, strlen((char*)InputBuffer), OutputBuffer, &OutputSize);

	delete InputBuffer;

	std::stringstream Result;
	for (unsigned int i = 0; i < OutputSize; ++i)
	{
		Result << std::hex << std::setfill('0') << std::setw(2) << std::nouppercase << (int)OutputBuffer[i];
		/*Result << std::hex << (int)OutputBuffer[i];*/
	}

	return Result.str();
}

std::string ZeuzAuth::GetDate()
{
	time_t RawTime;
	struct tm* TimeInfo;
	char Buffer[32];

	time(&RawTime);
	TimeInfo = gmtime(&RawTime);

	strftime(Buffer, 32, "%Y%m%dT%H%M%SZ", TimeInfo);

	std::string Result(Buffer);
	return Result;
}

std::string ZeuzAuth::GetStringToSign(std::vector<std::string> &Parameters, std::string Url)
{
	std::stringstream Result;
	for (std::string &Parameter : Parameters)
	{
		Result << Parameter;
	}
	Result << Url;
	return Result.str();
}

std::string ZeuzAuth::GetAccessToken()
{
	return _AccessToken;
}

std::string ZeuzAuth::GetAccessKey()
{
	return _AccessKey;
}

