#pragma once
#include "curl.h"
#include "url.h"

#define URL_HOST "localhost:5595"

CURLcode (*curl_setopt)(struct Curl_easy*, CURLoption, va_list) = nullptr;
CURLcode (*curl_easy_setopt)(struct Curl_easy*, CURLoption, ...) = nullptr;

CURLcode curl_setopt_(struct Curl_easy* data, CURLoption option, ...)
{
	va_list arg;
	va_start(arg, option);

	CURLcode result = curl_setopt(data, option, arg);

	va_end(arg);
	return result;
}

CURLcode curl_easy_setopt_detour(struct Curl_easy* data, CURLoption tag, ...)
{
	va_list arg;
	va_start(arg, tag);

	CURLcode result;

	if (!data) return CURLE_BAD_FUNCTION_ARGUMENT;

	if (tag == CURLOPT_SSL_VERIFYPEER)
	{
		result = curl_setopt_(data, tag, 0);
	}
#ifdef URL_HOST

	else if (tag == CURLOPT_URL)
	{
		std::string url = va_arg(arg, char*);
		size_t length = url.length();

		Uri uri = Uri::Parse(url);
		if (uri.Host.ends_with(".ol.epicgames.com"))
		{
			url = Uri::CreateUri("http", URL_HOST, uri.Port, uri.Path, uri.QueryString);
		}
		if (url.length() < length)
		{
			url.append(length - url.length(), ' '); // buffer size checking can occur
		}

		result = curl_setopt_(data, tag, url.c_str());
	}

#endif
	else
	{
		result = curl_setopt(data, tag, arg);
	}

	va_end(arg);
	return result;
}
