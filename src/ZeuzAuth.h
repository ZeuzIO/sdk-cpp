#pragma once

#include <iostream>
#include <algorithm>
#include <string> 
#include <stdio.h>
#include <json/json.h>
#include <vector>
#include <sstream>
#include <iomanip>
#include <bitset>

// SSL
#include <openssl/hmac.h>
#include <openssl/ossl_typ.h>
#include <openssl/evp.h>

#define ZEUZAUTH_ALGORITHM_SHA512 "sha512"

class ZeuzAuth
{
public:
	ZeuzAuth(std::string ZeuzAccessToken, std::string ZeuzAccessKey);
	virtual ~ZeuzAuth();

	// Zeuz Signature Generation
	std::string GenerateSignature(Json::Value &Requestbody, std::string Url);
	
	// Returns the list of parameters to sign
	void GetRequestParameters(Json::Value &RequestBody, std::vector<std::string> &OutParameters);
	
	// Implodes the parameters and the Url
	std::string GetStringToSign(std::vector<std::string> &Parameters, std::string Url);

	// Returns date in yyyymmddhhiiss format
	std::string GetDate();

	// Getter for the access token
	std::string GetAccessToken();

	// Getter for the access key
	std::string GetAccessKey();
private:
	const std::string _AccessToken;
	const std::string _AccessKey;
};