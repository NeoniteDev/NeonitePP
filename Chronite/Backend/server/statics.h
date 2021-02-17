#pragma once

inline void initStatics()
{
	//================================ 204s =================================
	app.Get("/", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

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

	app.Get(R"(/presence/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/party/api/v1/Fortnite/user/(.*)/notifications/undelivered/count)", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/eulatracking/api/public/agreements/fn/account/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get("/fortnite/api/game/v2/br-inventory/account/NeoniteUser", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/socialban/api/public/v1/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/api/v1/events/(.*))", [](const Request& req, Response& res)
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

	app.Get(R"(/party/api/v1/Fortnite/parties/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Delete(R"(/party/api/v1/Fortnite/parties/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/fortnite/api/game/v2/br-inventory/account/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Post(R"(/presence/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/fortnite/api/game/v2/matchmakingservice/ticket/(.*))", [](const Request& req, Response& res)
	{
		res.status = 204;
	});

	app.Get(R"(/statsproxy/api/(.*))", [](const Request& req, Response& res)
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

	app.Get(R"(/friends/api/v1/(.*)/recent/fortnite)", [](const Request& req, Response& res)
	{
		res.set_content("[]", "application/json");
	});

	app.Get(R"(/friends/api/v1/(.*)/blocklist)", [](const Request& req, Response& res)
	{
		res.set_content("[]", "application/json");
	});

	app.Get(R"(/fortnite/api/cloudstorage/user/(.*))", [](const Request& req, Response& res)
	{
		res.set_content("[]", "application/json");
	});
}
