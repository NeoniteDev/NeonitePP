#pragma once
#include "curl.h"
#include "url.h"

#define URL_PROTOCOL XOR("http")
#define URL_HOST XOR("localhost")
#define URL_PORT XOR("5595")

inline bool isReady = false;
inline std::string gUrl;
inline std::string gVersion;

static bool ProdMode = false;
static bool bIsProdMode;
static bool bIsVersionFound;

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
	else if (tag == CURLOPT_URL)
	{
		std::string url = va_arg(arg, char*);
		gUrl = url;

		if (url.find(XOR("ClientQuest")) != std::string::npos) isReady = !isReady;

#ifdef URL_HOST
		
		if(!ProdMode || url.find(XOR("cloudstorage")) != std::string::npos)
		{
			//printf("LogURL: %s\n", url.c_str());
			Uri uri = Uri::Parse(url);
			url = Uri::CreateUri(URL_PROTOCOL, URL_HOST, URL_PORT, uri.Path, uri.QueryString);
		}

#endif
		result = CurlSetOpt_(data, tag, url.c_str());
	}

		//Version determination
	else if (tag == CURLOPT_HTTPHEADER && !bIsVersionFound)
	{
		auto list = va_arg(arg, curl_slist*);;

		while (list->next != nullptr && list->data)
		{
			std::string data = list->data;
			if (data.starts_with("User-Agent:"))
			{
				const auto version = data.erase(0, 44).erase(5, data.size() - 5);
				gVersion = version;
				bIsVersionFound = !bIsVersionFound;
			}
			list = list->next;
		}
	}
	
	/*
	else if (tag == CURLOPT_HTTPHEADER && !ProdMode)
	{
		auto list = va_arg(arg, curl_slist*);;

		while (list->next != nullptr && list->data)
		{
			std::string data = list->data;
			if (data.starts_with("Authorization: bearer eg1"))
			{
				ProdMode = !ProdMode;
			}
			list = list->next;
		}
	}
	*/

	else
	{
		result = CurlSetOpt(data, tag, arg);
	}

	va_end(arg);
	return result;
}
