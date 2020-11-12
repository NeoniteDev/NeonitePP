#pragma once
#include "request.h"
#include "Routing/Router.h"
#include "dynamics.h"

inline std::map<std::string, http::response<http::dynamic_body>> responses;

inline Routing::Router router;

http::response<http::dynamic_body> cookResponse(http::request<http::string_body> req, http::response<http::dynamic_body> res, Routing::PathMatch match)
{
	auto route = match.pathTemplate();

	if (responses.count(route) > 0)
	{
		return responses[route];
	}
	return handleDynamic(match, req, res);
}
