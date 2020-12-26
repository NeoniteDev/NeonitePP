#pragma once
#include "curl.h"
#include "url.h"

#define URL_HOST "localhost"
#define URL_PORT "5595"

inline bool isReady = false;

CURLcode (*CurlSetOpt)(struct Curl_easy*, CURLoption, va_list) = nullptr;
CURLcode (*CurlEasySetOpt)(struct Curl_easy*, CURLoption, ...) = nullptr;

CURLcode CurlSetOpt_(struct Curl_easy* data, CURLoption option, ...)
{
	va_list arg;
	va_start(arg, option);

	CURLcode result = CurlSetOpt(data, option, arg);

	va_end(arg);
	return result;
}

CURLcode CurlEasySetOptDetour(struct Curl_easy* data, CURLoption tag, ...)
{
	va_list arg;
	va_start(arg, tag);

	CURLcode result;

	if (!data) return CURLE_BAD_FUNCTION_ARGUMENT;

	if (tag == CURLOPT_SSL_VERIFYPEER)
	{
		result = CurlSetOpt_(data, tag, 0);
	}

	//URL redirection
#ifdef URL_HOST	
	else if (tag == CURLOPT_URL)
	{
		std::string url = va_arg(arg, char*);

		if (url.find("ClientQuestLogin") != std::string::npos) isReady = !isReady;

		//printf("LogURL: %s\n", url.c_str());
		
		Uri uri = Uri::Parse(url);
		if (uri.Host.ends_with(".ol.epicgames.com"))
		{
			url = Uri::CreateUri("http", URL_HOST, URL_PORT, uri.Path, uri.QueryString);
		}
		
		result = CurlSetOpt_(data, tag, url.c_str());
	}

#endif
	else
	{
		result = CurlSetOpt(data, tag, arg);
	}

	va_end(arg);
	return result;
}
