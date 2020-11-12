#include "response.h"
#include "dynamics.h"

http::response<http::dynamic_body> cookResponse(http::request<http::string_body> req, http::response<http::dynamic_body> res, Routing::PathMatch match)
{
	auto route = match.pathTemplate();

	if (responses.count(route) > 0)
	{
		return responses[route];
	}
	else
	{
		return handleDynamic(match, req, res);
	}
}
