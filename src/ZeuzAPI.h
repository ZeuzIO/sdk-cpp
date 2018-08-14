#pragma once

#include <stdio.h>
#include "curl.h"
#include "ZeuzAuth.h"

#define ZEUZ_API_URL "https://api.zeuz.io/v2"

enum EZeuzAPIMethod
{
	GET,
	POST,
	PUT
};

class ZeuzAPI
{
public:
	ZeuzAPI(std::string ZeuzAccessToken, std::string ZeuzAccessKey);
	virtual ~ZeuzAPI();
	bool IsDebugging;
	long TimeOut;

	// Zeuz API Request (Returns a json object always even if failed)
	CURLcode APICall(EZeuzAPIMethod Method, std::string EndPoint, Json::Value &Request, Json::Value &Response);

	// Queries all services within a servergroup & game profile
	CURLcode ListServices(std::string ServerGroupId, std::string GameProfileId, Json::Value &OutServices);

	// Returns a single service by it's serviceId
	CURLcode GetService(std::string ServiceId, Json::Value &OutService);

	// Reserves the first available service by the servergroup and game profile
	CURLcode RequestService(std::string ServerGroupId, std::string GameProfileId, Json::Value &OutServices);
	
	// Reserves a service by it's service id
	CURLcode ReserveService(std::string ServiceId, Json::Value &OutService);
	
	// Unreserves a service by it's service id
	CURLcode UnreserveService(std::string ServiceId, Json::Value &OutService);

	// Returns the status of the service (Is the service running?)
	CURLcode GetStatus(std::string ServiceId, Json::Value &OutService);

	// Spawns a new unmanaged service on the first available hardware
	CURLcode AllocateService(std::string ServerGroupId, std::string GameProfileId, Json::Value &OutService);

	// Destroys a previously allocated service
	CURLcode UnallocateService(std::string ServiceId, Json::Value &OutService);

	// Starts a service if it was stopped
	CURLcode Start(std::string ServiceId, Json::Value &OutService);

	// Stops a running service,
	CURLcode Stop(std::string ServiceId, Json::Value &OutService);

	// Restarts a service, if it was stopped it will be started
	CURLcode Restart(std::string ServiceId, Json::Value &OutService);

	// Stops the service if its running and forces a reinstall,
	CURLcode Reinstall(std::string ServiceId, Json::Value &OutService);

	// Send CCU / Statistics information to the Zeuz API
	CURLcode PostStatistics(std::string ServiceId, int playerCount, int maxPlayerCount, Json::Value &OutService);

	// Get CCU / Statistics Information about the Zeuz API
	CURLcode GetStatistics(std::string ServiceId, Json::Value &OutService);
private:
	ZeuzAuth *_ZeuzAuth;
};