#include "response.h"
#include "../../includes/http/HTTPRequest.hpp"

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

inline http::response<http::dynamic_body> handleDynamic(
	Routing::PathMatch match, http::request<http::string_body> req,
	http::response<http::dynamic_body> res)
{
	auto route = match.pathTemplate();
	switch (str2int(route.c_str()))
	{
	case str2int("/fortnite/api/game/v2/privacy/account/:accountId"):
	{
		auto accountId = match["accountId"];
		json j = {
			{"accountId", accountId},
			{"optOutOfPublicLeaderboards", false}
		};
		res.set(http::field::content_type, "application/json");
		ostream(res.body()) << j;
		return res;
	}

	//oauth
	case str2int("/account/api/public/account/:accountId/deviceAuth"):
	{
		if (req.method() == http::verb::get)
		{
			res.result(http::status::no_content);
			return res;
		}
		else
		{
			auto accountId = match["accountId"];
			json j = {
				{"accountId", accountId},
				{"deviceId", "aabbccddeeff11223344556677889900"},
				{"secret", "aabbccddeeff11223344556677889900"}
			};
			res.set(http::field::content_type, "application/json");
			ostream(res.body()) << j;
			return res;
		}
	}

	case str2int("/account/api/public/account/:accountId"):
	{
		auto accountId = match["accountId"];
		json j = {
			{"id", accountId},
			{"displayName", accountId},
			{"externalAuths", json({})}
		};
		res.set(http::field::content_type, "application/json");
		ostream(res.body()) << j;
		return res;
	}

	case str2int("/account/api/oauth/verify"):
	{
		const char* authorization = req["Authorization"].data();
		std::string token = authorization;
		token.erase(token.begin(), token.begin() + 7);
		json j = {
			{"access_token", token},
			{"expires_in", 28800},
			{"expires_at","9999 - 12 - 31T23:59 : 59.999Z"},
			{"token_type", "bearer"},
			{"refresh_token", "neonitepprefreshtokenplaceholder"},
			{"refresh_expires", 115200},
			{"refresh_expires_at", "9999-12-31T23:59:59.999Z"},
			{"account_id", "neonitepp"},
			{"client_id", "neoniteppclientidplaceholder"},
			{"internal_client", "true"},
			{"client_service", "fortnite"},
			{"displayName", "neoniteppplayer"},
			{"app", "fortnite"},
			{"in_app_id", "neoniteppidplaceholder"},
			{"device_id", "neoniteppdeviceidplaceholder"}
		};
		res.set(http::field::content_type, "application/json");
		ostream(res.body()) << j;
		return res;
	}
	}
	throw;
}
