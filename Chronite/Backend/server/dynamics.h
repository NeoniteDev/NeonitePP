#pragma once

inline void initDynamics()
{
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

	app.Get("/content/api/pages/fortnite-game/", [](const Request& req, Response& res)
	{
		json j = fortniteGame();
		res.set_content(j.dump(), "application/json");
	});

	app.Get("/fortnite/api/calendar/v1/timeline", [](const Request& req, Response& res)
	{
		json j = calendar();
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

	app.Get(R"(/party/api/v1/Fortnite/user/(.*))", [](const Request& req, Response& res)
	{
		const auto j = R"({ "current": [], "pending": [], "invites": [], "pings": [] })"_json;
		res.set_content(j.dump(), "application/json");
	});
}
