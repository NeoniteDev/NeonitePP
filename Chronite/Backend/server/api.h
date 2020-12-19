#pragma once

inline void initApi()
{
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

	//OAuth
	app.Get("/account/api/oauth/verify", [](const Request& req, Response& res)
	{
		auto token = static_cast<std::string>(req.get_header_value("Authorization"));
		token.erase(token.begin(), token.begin() + 7);
		json j = {
			{"access_token", token},
			{"expires_in", 28800},
			{"expires_at", "9999-12-31T23:59:59.999Z"},
			{"token_type", "bearer"},
			{"refresh_token", "neonitepprefreshtokenplaceholder"},
			{"refresh_expires", 115200},
			{"refresh_expires_at", "9999-12-31T23:59:59.999Z"},
			{"account_id", "neonitepp"},
			{"client_id", "neoniteppclientidplaceholderkekw"},
			{"internal_client", "true"},
			{"client_service", "fortnite"},
			{"displayName", "neoniteppplayer"},
			{"app", "fortnite"},
			{"in_app_id", "neonitepp"},
			{"device_id", "neoniteppdeviceidplaceholderkekw"}
		};
		res.set_content(j.dump(), "application/json");
	});

	app.Post("/account/api/oauth/token", [](const Request& req, Response& res)
	{
		json j;
		if (util::getQuery(req.body, "grant_type") == "client_credentials")
		{
			j = {
				{"access_token", util::genRandom(32)},
				{"expires_in", 28800},
				{"expires_at", "9999-12-31T23:59:59.999Z"},
				{"token_type", "bearer"},
				{"client_id", "neoniteppclientidplaceholderkekw"},
				{"internal_client", "true"},
				{"client_service", "fortnite"},
			};
		}
		else
		{
			std::string displayName;
			std::string body = util::urlDecode(req.body);

			if (util::getQuery(body, "email") != "failed")
			{
				displayName = util::sSplit(util::getQuery(body, "email"), "@");
			}
			else if (util::getQuery(body, "username") != "failed")
			{
				displayName = util::sSplit(util::getQuery(body, "username"), "@");
			}
			else displayName = "NeonitePPUser";

			j = {
				{"access_token", util::genRandom(32)},
				{"expires_in", 28800},
				{"expires_at", "9999-12-31T23:59:59.999Z"},
				{"token_type", "bearer"},
				{"account_id", displayName},
				{"client_id", "neoniteppclientidplaceholderkekw"},
				{"internal_client", true},
				{"client_service", "fortnite"},
				{"displayName", displayName},
				{"app", "fortnite"},
				{"in_app_id", displayName},
				{"device_id", "neonitedeviceidplaceholderkekw"}
			};
		}

		res.set_content(j.dump(), "application/json");
	});
}
