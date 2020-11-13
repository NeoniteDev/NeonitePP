#include "common_core.h"
#include "response.h"

std::string querySplit(std::string s, std::string query)
{
	size_t pos = 0;
	std::string token;
	if ((pos = s.find(query)) != std::string::npos)
	{
		token = s.substr(pos);
		token.erase(0, query.length() + 1);
		size_t end;
		try
		{
			end = token.find("&");
			token.erase(end);
			return token;
		}
		catch (...)
		{
			return token;
		}
	}
}


inline http::response<http::dynamic_body> handleProfile(
	Routing::PathMatch match,
	http::request<http::string_body> req,
	http::response<http::dynamic_body> res)
{
	json body = json::parse(req.body());
	json profileData;
	std::string profileId;
	int rvn;
	int profileCommandRvn;
	int profileRvn;
	std::string accountId = match["accountId"];
	std::string command = match["command"];

	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	auto date = to_iso_extended_string(t);

	//TONS OF TRY STATMENT :PEPE_KMS:
	try
	{
		profileId = querySplit(match.path(), "profileId");
	}
	catch (...)
	{
		profileId = "common_core";
	}

	try
	{
		rvn = stoi(querySplit(match.path(), "rvn"));
	}
	catch (...)
	{
		rvn = -1;
	}

	//TODO: handle commands.

	if (profileId == "common_core")
	{
		profileData = common_core();
	}

	try
	{
		profileRvn = profileData["rvn"];
	}
	catch (...)
	{
		profileRvn = 1;
	}

	try
	{
		profileCommandRvn = profileData["commandRevision"];
	}
	catch (...)
	{
		profileCommandRvn = 1;
	}

	res.set(http::field::content_type, "application/json");
	json response = {
		{"profileRevision", profileRvn},
		{"profileId", profileId},
		{"profileChangesBaseRevision", profileRvn},
		{"profileChanges", json::array()},
		{"serverTime", date},
		{"profileCommandRevision", profileCommandRvn},
		{"responseVersion", 1}
	};
	
	profileData["rvn"] = rvn + 1;
	profileData["updated"] = date;
	profileData["commandRevision"] = profileCommandRvn + 1;

	response["profileRevision"] = profileData["rvn"];
	response["profileCommandRevision"] = profileData["commandRevision"];

	if (rvn != profileRvn)
	{
		response["profileChanges"] = json::array({{"changeType", "fullProfileUpdate"}, {"profile", profileData}});
	}

	ostream(res.body()) << response;
	return res;
}
