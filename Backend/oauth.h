#include "response.h"

std::string gen_random(const int len)
{
	std::string tmp_s;
	static const char alphanum[] =
	"0123456789"
	"abcdefghijklmnopqrstuvwxyz";

	srand((unsigned)time(NULL) * _getpid());

	for (int i = 0; i < len; ++i) tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


	return tmp_s;
}

std::string split(std::string s, std::string delimiter)
{
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		return token;
	}
}

inline http::response<http::dynamic_body> handleToken(
	http::request<http::string_body> req,
	http::response<http::dynamic_body> res)
{
	json body = json::parse(req.body());
	std::string rawDisplayName;
	try
	{
		rawDisplayName = body["email"].get<std::string>();
	}
	catch (...)
	{
		//:bigbrain: -kemo
		rawDisplayName = body["username"].get<std::string>();
	}


	auto displayName = split(rawDisplayName, "@");

	res.set(http::field::content_type, "application/json");
	json j = {
		{"access_token", gen_random(32)},
		{"expires_in", 9999999},
		{"expires_at", "9999-12-31T23:59:59.999Z"},
		{"token_type", "bearer"},
		{"account_id", displayName},
		{"client_id", "neoniteppclientidplaceholder"},
		{"internal_client", "true"},
		{"client_service", "fortnite"},
		{"displayName", displayName},
		{"app", "fortnite"},
		{"in_app_id", displayName},
		{"device_id", "neoniteppdeviceidplaceholder"}
	};
	ostream(res.body()) << j;
	return res;
}
