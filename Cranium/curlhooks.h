#pragma once
#include <regex>

#include "curl.h"
#include "url.h"
#include "veh.h"

#define URL_PROTOCOL XOR("http")
#define URL_HOST XOR("127.0.0.1")
#define URL_PORT XOR("5595")

inline bool isReady = false;
inline std::string gUrl;
inline std::string gVersion;

static bool ProdMode = false;
static bool bIsProdMode;
static bool bIsVersionFound;

inline uintptr_t CurlVsetoptAddress;
inline VEH::Hook* hCurlVsetoptHook;

CURLcode (*CurlVsetopt_)(LPVOID, CURLoption, va_list) = nullptr;

CURLcode CurlVsetoptHook(LPVOID lpContext, CURLoption iOption, va_list param); // Forward declaring this is necessary!!

CURLcode CurlVsetopt(LPVOID lpContext, CURLoption iOption, va_list param)
{
	hCurlVsetoptHook->~Hook();
	
	CURLcode iResult = CurlVsetopt_(lpContext, iOption, param);

	hCurlVsetoptHook = new VEH::Hook(CurlVsetoptAddress, reinterpret_cast<uintptr_t>(CurlVsetoptHook));

	if (!hCurlVsetoptHook->bSuccess)
	{
		printf("Reinstalling hook for CurlVsetopt has failed, exiting immediately!\n");
		exit(EXIT_FAILURE);
	}

	return iResult;
}

CURLcode CurlVsetoptVa(LPVOID lpContext, CURLoption iOption, ...)
{
	va_list list{};
	va_start(list, iOption);

	CURLcode iResult = CurlVsetopt(lpContext, iOption, list);

	va_end(list);

	return iResult;
}

CURLcode CurlVsetoptHook(LPVOID data, CURLoption tag, va_list param)
{
	va_list arg;
	va_copy(arg, param);

	CURLcode result;

	if (!data) return CURLE_BAD_FUNCTION_ARGUMENT;

	if (tag == CURLOPT_SSL_VERIFYPEER)
	{
		result = CurlVsetoptVa(data, tag, 0);
	}

	if (tag == CURLOPT_SSL_VERIFYHOST)
	{
		result = CurlVsetoptVa(data, tag, 0);
	}

	if (tag == CURLOPT_PROXY_SSL_VERIFYPEER)
	{
		result = CurlVsetoptVa(data, tag, 0);
	}

	if (tag == CURLOPT_PROXY_SSL_VERIFYHOST)
	{
		result = CurlVsetoptVa(data, tag, 0);
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
			printf("LogURL: %s\n", url.c_str());
			Uri uri = Uri::Parse(url);
			url = Uri::CreateUri(URL_PROTOCOL, URL_HOST, URL_PORT, uri.Path, uri.QueryString);
			printf("LogURL: %s\n", url.c_str());
		}

#endif
		result = CurlVsetoptVa(data, tag, url.c_str());
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
		result = CurlVsetopt(data, tag, arg);
	}

	va_end(arg);
	return result;
}
