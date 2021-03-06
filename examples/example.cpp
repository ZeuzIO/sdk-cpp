// ZeuzApiCallsTest.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include "curl.h"
#include "ZeuzAPI.h"

#define ACCESS_KEY "YOUR_ACCESS_KEY"
#define ACCESS_TOKEN "YOUR_ACCESS_TOKEN"

void getservice_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	// Get information about a service (Usually you would use externalIp and queryPort/gamePort for connecting, primaryIp is used for the game server to listen in the internal network)
	Json::Value ServiceData;
	zeuzApi.GetService("123", ServiceData);
	std::cout << "Request: " << ServiceData["request"]["uuid"].asString() << " Success: " << ServiceData["success"].asString() << std::endl;
	if (ServiceData["success"].asBool())
	{
		std::cout << "Server Address: " << ServiceData["service"]["externalIp"].asString() << ":" << ServiceData["service"]["queryPort"].asString() << std::endl;
	}
	else {
		std::cout << "Service not found" << std::endl;
	}

	// Check if there was an error
	if (ServiceData["error"].asBool())
	{
		std::cout << "Failed With Error: " << ServiceData["message"].asString() << std::endl;
	}
}

void reserve_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	// Reserve a service
	Json::Value ReserveData;
	zeuzApi.ReserveService("123", ReserveData);
	std::cout << "Request: " << ReserveData["request"]["uuid"].asString() << " Success: " << ReserveData["success"].asString() << std::endl;
	if (ReserveData["success"].asBool())
	{
		std::cout << "Reserved Service Address: " << ReserveData["service"]["externalIp"].asString() << ":" << ReserveData["service"]["queryPort"].asString() << std::endl;
	}
	else {
		std::cout << "Failed to reserve the service" << std::endl;
	}

	// Check if there was an error
	if (ReserveData["error"].asBool())
	{
		std::cout << "Failed With Error: " << ReserveData["message"].asString() << std::endl;
	}
}

void unreserve_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	// UnReserve a service
	Json::Value UnReserveData;
	zeuzApi.UnreserveService("123", UnReserveData);
	std::cout << "Request: " << UnReserveData["request"]["uuid"].asString() << " Success: " << UnReserveData["success"].asString() << std::endl;
	if (UnReserveData["success"].asBool())
	{
		std::cout << "Unreserved Service Address: " << UnReserveData["service"]["externalIp"].asString() << ":" << UnReserveData["service"]["queryPort"].asString() << std::endl;
	}
	else {
		std::cout << "Failed to unreserve the service" << std::endl;
	}

	// Check if there was an error
	if (UnReserveData["error"].asBool())
	{
		std::cout << "Failed With Error: " << UnReserveData["message"].asString() << std::endl;
	}
}

void getstatus_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	// Get the status of the server (Is it running, or still preparing?) Can be polled for live information
	Json::Value StatusData;
	zeuzApi.GetStatus("123", StatusData);
	std::cout << "Request: " << StatusData["request"]["uuid"].asString() << " Success: " << StatusData["success"].asString() << std::endl;
	if (StatusData["success"].asBool())
	{
		std::cout << "Service Status:" << StatusData["status"].asString()  << std::endl;
	}
	else {
		std::cout << "Failed to get status of the service" << std::endl;
	}

	// Check if there was an error
	if (StatusData["error"].asBool())
	{
		std::cout << "Failed With Error: " << StatusData["message"].asString() << std::endl;
	}
}

void requestservice_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	// Reserve the first available service from a servergroup + gameprofile combination (In case it fails, it is safe to retry to get another service with this request)
	Json::Value RequestData;
	zeuzApi.RequestService("1", "1", RequestData);
	std::cout << "Request: " << RequestData["request"]["uuid"].asString() << " Success: " << RequestData["success"].asString() << std::endl;
	if (RequestData["success"].asBool())
	{
		std::cout << "ServiceId: " << RequestData["service"]["serviceId"].asString() << " Server Address: " << RequestData["service"]["externalIp"].asString() << ":" << RequestData["service"]["queryPort"].asString() << std::endl;
	}
	else {
		std::cout << "Failed to get reserve a service" << std::endl;
	}

	// Check if there was an error
	if (RequestData["error"].asBool())
	{
		std::cout << "Failed With Error: " << RequestData["message"].asString() << std::endl;
	}
}

void postccu_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	//Json::Value StatisticsResponse;
	zeuzApi.PostStatistics("123", 16, 32, StatisticsResponse);
	std::cout << "Request: " << StatisticsResponse["request"]["uuid"].asString() << " Success: " << StatisticsResponse["success"].asString() << std::endl;
	if (StatisticsResponse["success"].asBool())
	{
		std::cout << "CCU On Service:" << StatisticsResponse["statistics"]["playerCount"] << std::endl;
	}
	else {
		std::cout << "Failed to get reserve a service" << std::endl;
	}

	// Check if there was an error
	if (StatisticsResponse["error"].asBool())
	{
		std::cout << "Failed With Error: " << StatisticsResponse["message"].asString() << std::endl;
	}
}

void getccu_example()
{
	ZeuzAPI zeuzApi(ACCESS_TOKEN, ACCESS_KEY);

	// Get CCU Information from the Zeuz API about a service
	Json::Value StatisticsRequest;
	zeuzApi.GetStatistics("123", StatisticsRequest);
	std::cout << "Request: " << StatisticsRequest["request"]["uuid"].asString() << " Success: " << StatisticsRequest["success"].asString() << std::endl;
	if (StatisticsRequest["success"].asBool())
	{
		std::cout << "CCU On Service:" << StatisticsRequest["statistics"]["playerCount"] << std::endl;
	}
	else {
		std::cout << "Failed to get reserve a service" << std::endl;
	}

	// Check if there was an error
	if (StatisticsRequest["error"].asBool())
	{
		std::cout << "Failed With Error: " << StatisticsRequest["message"].asString() << std::endl;
	}
}
