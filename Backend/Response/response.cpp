#include "response.h"

void registerRoutes() {
  http::response<http::dynamic_body> res;
  std::string route;

  route = "/lightswitch/api/service/bulk/status";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      <<
      R"(
        {
            "serviceInstanceId": "fortnite",
            "status": "UP",
            "message": "Neonite++",
            "maintenanceUri": null,
            "allowedActions": ["PLAY", "DOWNLOAD"],
            "banned": false
        }
        )"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  route = "/account/api/public/account/:accountId/externalAuths";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"([])"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  route = "/fortnite/api/v2/versioncheck/:version";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"({"type": "NO_UPDATE"})"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/party/api/v1/Fortnite/user/:accountId";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"({
	current: [],
	pending: [],
	invites: [],
	pings: []
    })"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/fortnite/api/matchmaking/session/findPlayer/:id";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"([])"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/party/api/v1/Fortnite/user/:accountId/notifications/undelivered/count";
  res.result(http::status::no_content);
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/presence/api/v1/_/:accountId/settings/subscriptions";
  res.result(http::status::no_content);
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/socialban/api/public/v1/:accountId";
  res.result(http::status::no_content);
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/content-controls/:accountId";
  res.result(http::status::no_content);
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/fortnite/api/game/v2/tryPlayOnPlatform/account/:accountId";
  res.set(http::field::content_type, "text/plain");
  beast::ostream(res.body()) << true;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/waitingroom/api/waitingroom";
  res.result(http::status::no_content);
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/fortnite/api/game/v2/grant_access/:accountId";
  res.result(http::status::no_content);
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/fortnite/api/receipts/v1/account/:accountId/receipts";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"([])"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/fortnite/api/game/v2/enabled_features";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"([])"_json;  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/friends/api/public/blocklist/:accountId";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"({blockedUsers: []})"_json;
  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/friends/api/v1/:accountId/settings";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"({acceptInvites: "public"})"_json;  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/friends/api/public/friends/:accountId";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"([])"_json;  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();

  // * sammy added
  route = "/friends/api/public/list/fortnite/:accountId/recentPlayers";
  res.set(http::field::content_type, "application/json");
  beast::ostream(res.body())
      << R"([])"_json;  router.registerPath(route);
  responses.insert(
      std::pair<std::string, http::response<http::dynamic_body>>(route, res));
  res.body().clear();




}

http::response<http::dynamic_body> cookResponse(
    http::request<http::string_body> req,
    http::response<http::dynamic_body> res, Routing::PathMatch match) {
  auto route = match.pathTemplate();
  //static endpoints are hard coded in responses map
  if (responses.count(route) > 0) {
    return responses[route];
  }
    //dynamic endpoints
  else if (route == "") {
    //i wanna die ok
  } else {
    throw;
  }

  return res;
}
