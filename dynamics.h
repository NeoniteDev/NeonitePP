#include "response.h"

constexpr unsigned int str2int(const char* str, int h = 0)
{
	return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

inline http::response<http::dynamic_body> handleDynamic(Routing::PathMatch match, http::request<http::string_body> req, http::response<http::dynamic_body> res)
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
		beast::ostream(res.body()) << j;
		return res;
	}

	case str2int(""):
	{
	}
	}
	throw;
}
