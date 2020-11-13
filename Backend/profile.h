#include "response.h"

string querySplit(string s, string delimiter)
{
	size_t pos = 0;
	string token;
	if ((pos = s.find(delimiter)) != string::npos)
	{
		size_t end = s.find("&");
		token = s.substr(pos, end);
		token.erase(0, delimiter.length());
		cout << token << endl;
	}
}

inline http::response<http::dynamic_body> handleProfile(
	Routing::PathMatch match,
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
