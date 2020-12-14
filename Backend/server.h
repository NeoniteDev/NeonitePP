#pragma once
#include "common_core.h"
#include "../util.h"

using namespace httplib;

inline HANDLE hServer = INVALID_HANDLE_VALUE;
inline Server app;

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

std::string get_query(std::string s, std::string query)
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
	return "failed";
}

std::string url_decode(std::string SRC)
{
	std::string ret;
	char ch;
	int i, ii;
	for (i = 0; i < SRC.length(); i++)
	{
		if (int(SRC[i]) == 37)
		{
			sscanf(SRC.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
		else
		{
			ret += SRC[i];
		}
	}
	return (ret);
}


namespace server
{
	inline void init()
	{
		//yes one file for now. -kemo

		//================================ 204s =================================
		app.Get("/waitingroom/api/waitingroom", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Get(R"(/fortnite/api/game/v2/grant_access/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Post(R"(/datarouter/api/v1/public/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Get(R"(/presence/api/v1/_/(.*)/settings/subscriptions)", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Get(R"(/party/api/v1/Fortnite/user/(.*)/notifications/undelivered/count)", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Get(R"(/socialban/api/public/v1/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Get(R"(/content-controls/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Get(R"(/fortnite/api/cloudstorage/user/(.*)/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Put(R"(/fortnite/api/cloudstorage/user/(.*)/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Delete("/account/api/oauth/sessions/kill", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		app.Delete(R"(/account/api/oauth/sessions/kill/(.*))", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		//================================ Empty Arrays =================================

		app.Get(R"(/account/api/public/account/(.*)/externalAuths)", [](const Request& req, Response& res)
		{
			res.set_content("[]", "application/json");
		});

		app.Get("/fortnite/api/game/v2/enabled_features", [](const Request& req, Response& res)
		{
			res.set_content("[]", "application/json");
		});

		app.Get(R"(/fortnite/api/receipts/v1/account/(.*)/receipts)", [](const Request& req, Response& res)
		{
			res.set_content("[]", "application/json");
		});

		app.Get(R"(/friends/api/public/list/fortnite/(.*)/recentPlayers)", [](const Request& req, Response& res)
		{
			res.set_content("[]", "application/json");
		});

		app.Get(R"(/friends/api/public/friends/(.*))", [](const Request& req, Response& res)
		{
			res.set_content("[]", "application/json");
		});

		//================================ Dynamics =================================

		app.Get("/lightswitch/api/service/bulk/status", [](const Request& req, Response& res)
		{
			const auto j =
			R"(
        [{
            "serviceInstanceId": "fortnite",
            "status": "UP",
            "message": "Neonite++",
            "maintenanceUri": null,
            "allowedActions": ["PLAY", "DOWNLOAD"],
            "banned": false
        }]
        )"_json;
			res.set_content(j.dump(), "application/json");
		});

		app.Get(R"(/fortnite/api/v2/versioncheck/(.*))", [](const Request& req, Response& res)
		{
			const auto j = R"({"type": "NO_UPDATE"})"_json;
			res.set_content(j.dump(), "application/json");
		});

		app.Get("/account/api/public/account", [](const Request& req, Response& res)
		{
			auto account_id = req.get_param_value("accountId");
			json j = {
				{"id", account_id},
				{"displayName", account_id},
				{"externalAuths", json({})}
			};
			res.set_content(j.dump(), "application/json");
		});

		app.Get(R"(/account/api/public/account/(.*))", [](const Request& req, Response& res)
		{
			auto account_id = static_cast<std::string>(req.matches[1]);
			json j = {
				{"id", account_id},
				{"displayName", account_id},
				{"externalAuths", json({})}
			};
			res.set_content(j.dump(), "application/json");
		});

		app.Get(R"(/fortnite/api/game/v2/privacy/account/(.*))", [](const Request& req, Response& res)
		{
			auto account_id = static_cast<std::string>(req.matches[1]);
			json j = {
				{"accountId", account_id},
				{"optOutOfPublicLeaderboards", false}
			};
			res.set_content(j.dump(), "application/json");
		});

		app.Get(R"(/friends/api/public/blocklist/(.*))", [](const Request& req, Response& res)
		{
			const auto j = R"({ "blockedUsers": [] })"_json;
			res.set_content(j.dump(), "application/json");
		});

		app.Get(R"(/friends/api/v1/(.*)/settings)", [](const Request& req, Response& res)
		{
			const auto j = R"({ "blockedUsers": [] })"_json;
			res.set_content(j.dump(), "application/json");
		});

		app.Post(R"(/fortnite/api/game/v2/tryPlayOnPlatform/account/(.*))", [](const Request& req, Response& res)
		{
			res.set_content("true", "text/plain");
		});

		app.Get(R"(/affiliate/api/public/affiliates/slug/(.*))", [](const Request& req, Response& res)
		{
			const auto j =
			R"(
        {
            "id": "aabbccddeeff11223344556677889900",
            "slug": "neonite++",
            "displayName": "neonite++",
	        "status": "ACTIVE",
	        "verified": true
        }
        )"_json;
			// sorry ytrs :pepelaugh: -kemo
			res.set_content(j.dump(), "application/json");
		});

		//==> TO BE IMPLEMENTED <==
		app.Get("/fortnite/api/cloudstorage/system", [](const Request& req, Response& res)
		{
			res.status = 204;
		});

		//==> TO BE IMPLEMENTED <==
		app.Get(R"(/fortnite/api/cloudstorage/user/(.*))", [](const Request& req, Response& res)
		{
			res.set_content("[]", "application/json");
		});

		//==> TO BE IMPLEMENTED <==
		app.Get("/fortnite/api/storefront/v2/keychain", [](const Request& req, Response& res)
		{
			const auto j = R"(["A93064DA8BDA456CADD2CD316BE64EE5:nziBPQTfuEl4IRK6pOaovQpqQC6nsMQZFTx+DEg62q4=:CID_NEONITE_TEMP"])"_json;
			res.set_content(j.dump(), "application/json");
		});

		app.Get(R"(/party/api/v1/Fortnite/user/(.*))", [](const Request& req, Response& res)
		{
			const auto j = R"({ "current": [], "pending": [], "invites": [], "pings": [] })"_json;
			res.set_content(j.dump(), "application/json");
		});

		//OAuth
		app.Get("/account/api/oauth/verify", [](const Request& req, Response& res)
		{
			auto token = static_cast<std::string>(req.get_header_value("Authorization"));
			token.erase(token.begin(), token.begin() + 7);
			json j = {
				{"access_token", token},
				{"expires_in", 999999999},
				{"expires_at", "9999-12-31T23:59:59.999Z"},
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
				{"in_app_id", "neonitepp"},
				{"device_id", "neoniteppdeviceidplaceholder"}
			};
			res.set_content(j.dump(), "application/json");
		});

		app.Post("/account/api/oauth/token", [](const Request& req, Response& res)
		{
			json j;
			if (get_query(req.body, "grant_type") == "client_credentials")
			{
				j = {
					{"access_token", gen_random(32)},
					{"expires_in", 9999999},
					{"expires_at", "9999-12-31T23:59:59.999Z"},
					{"token_type", "bearer"},
					{"client_id", "neoniteppclientidplaceholder"},
					{"internal_client", "true"},
					{"client_service", "fortnite"},
				};
			}
			else
			{
				std::string displayName;
				std::string body = url_decode(req.body);

				if (get_query(body, "email") != "failed")
				{
					displayName = split(get_query(body, "email"), "@");
				}
				else if (get_query(body, "username") != "failed")
				{
					displayName = split(get_query(body, "username"), "@");
				}
				else displayName = "NeonitePPUser";

				j = {
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
			}

			res.set_content(j.dump(), "application/json");
		});

		//Profile
		app.Post(R"(/fortnite/api/game/v2/profile/(.*)/client/(.*))", [](const Request& req, Response& res)
		{
			std::string account_id = static_cast<std::string>(req.matches[1]);
			std::string command = static_cast<std::string>(req.matches[2]);
			std::string profile_id = "common_core";
			int rvn = -1;
			boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
			auto date = to_iso_extended_string(t);
			json profileData;

			if (req.has_param("rvn")) rvn = std::stoi(req.get_param_value("rvn"));
			if (req.has_param("profileId")) profile_id = req.get_param_value("profileId");
			if (profile_id == "common_core") profileData = common_core();

			json response = {
				{"profileRevision", profileData["rvn"]},
				{"profileId", profile_id},
				{"profileChangesBaseRevision", profileData["rvn"]},
				{"profileChanges", json::array()},
				{"serverTime", date},
				{"profileCommandRevision", profileData["commandRevision"]},
				{"responseVersion", 1}
			};

			if (response.size() > 0)
			{
				profileData["rvn"] = rvn + 1;
				response["profileRevision"] = profileData["rvn"];
				response["profileCommandRevision"] = profileData["commandRevision"];
			}

			if (rvn != response["profileChangesBaseRevision"])
			{
				response["profileChanges"] = json::array({
						{"changeType", "fullProfileUpdate"},
						{"profile", profileData}
					});
			};

			res.set_content(response.dump(), "application/json");
		});

		app.listen("127.0.0.1", 5595);
	}
}
