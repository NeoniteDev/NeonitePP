#pragma once
#include "../Frontend/util.h"
#include "response.h"

using namespace Routing;

inline http::response<http::dynamic_body> handle_request(
	http::request<http::string_body> req,
	http::response<http::dynamic_body> res) {
	std::string route = (std::string)req.target();

	/*Headers: req["Authorization"]
	*
	json body;
	if (req.method() != http::verb::get) {
		try {
			body = json::parse(req.body());
		}
		catch (...) {
			//body isn't json ¯\_(ツ)_/¯
		}
	}

	*/
	//TODO: maybe move this some where.
	if (strstr(route.c_str(), "/datarouter/api/v1/public/")) {
		res.result(http::status::no_content);
		return res;
	}

	try {
		PathMatch match = router.matchPath(route);
		return cookResponse(req, res, match);
	}
	catch (...) {
		res.result(http::status::not_found);
		res.set(http::field::content_type, "text/plain");
		beast::ostream(res.body()) << "Endpoint is not a registered route\r\n";
		return res;
	}
}