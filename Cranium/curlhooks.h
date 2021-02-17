#pragma once
#include "curl.h"
#include "url.h"
#include "veh.h"

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
CURLcode CurlSetOptDetour(struct Curl_easy* data, CURLoption tag, ...);

inline uintptr_t CurlVsetoptAddress;
inline VEH::Hook* CurlVsetoptHook;

CURLcode CurlSetOpt_(struct Curl_easy* data, CURLoption option, ...)
{
	va_list arg;
	va_start(arg, option);

	CurlVsetoptHook->~Hook();

	printf("uhh");

	CURLcode result = CurlSetOpt(data, option, arg);

	CurlVsetoptHook = new VEH::Hook(CurlVsetoptAddress, reinterpret_cast<uintptr_t>(CurlSetOptDetour));
	if (!CurlVsetoptHook->bSuccess)
	{
		printf("Reinstalling hook for CurlVsetopt has failed, exiting immediately!\n");
		exit(EXIT_FAILURE);
	}

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

		if (!ProdMode || url.find(XOR("cloudstorage")) != std::string::npos)
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
				auto version = data.erase(0, 44).erase(5, data.size() - 5);
				gVersion = version;
				bIsVersionFound = !bIsVersionFound;
			}
			list = list->next;
		}
	}

	else
	{
		result = CurlSetOpt(data, tag, arg);
	}

	va_end(arg);
	return result;
}


CURLcode CurlSetOptDetour(struct Curl_easy* data, CURLoption tag, ...)
{
	va_list arg;
	va_start(arg, tag);

	CURLcode result;

	switch (tag)
	{
	case CURLOPT_SSL_VERIFYPEER:
	{
		result = CurlSetOpt_(data, tag, 0);
		break;
	}

	case CURLOPT_URL: // CURLOPT_URL
	{
		std::string url = va_arg(arg, char*);
		gUrl = url;

		if (url.find(XOR("ClientQuest")) != std::string::npos) isReady = !isReady;

#ifdef URL_HOST

		if (!ProdMode || url.find(XOR("cloudstorage")) != std::string::npos)
		{
			printf("LogURL: %s\n", url.c_str());
			Uri uri = Uri::Parse(url);
			url = Uri::CreateUri(URL_PROTOCOL, URL_HOST, URL_PORT, uri.Path, uri.QueryString);
		}

#endif
		result = CurlSetOpt_(data, tag, url.c_str());
		break;
	}

	case CURLOPT_HTTPHEADER:
	{
		if (!bIsVersionFound)
		{
			auto list = va_arg(arg, curl_slist*);;

			while (list->next != nullptr && list->data)
			{
				std::string data = list->data;
				if (data.starts_with("User-Agent:"))
				{
					auto version = data.erase(0, 44).erase(5, data.size() - 5);
					gVersion = version;
					bIsVersionFound = !bIsVersionFound;
				}
				list = list->next;
			}
		}

		break;
	}

	default:
		result = CurlSetOpt(data, tag, arg);
		break;
	}

	va_end(arg);
	return result;
}
