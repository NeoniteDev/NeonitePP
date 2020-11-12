#pragma once
#include "request.h"
#include "Routing/Router.h"

inline std::map<std::string, http::response<http::dynamic_body>> responses;

inline Routing::Router router;
void registerRoutes();
http::response<http::dynamic_body> cookResponse(http::request<http::string_body> req, http::response<http::dynamic_body> res, Routing::PathMatch match);