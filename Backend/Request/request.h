#pragma once
#include "../../Frontend/util.h"

http::response<http::dynamic_body> handle_request(
    http::request<http::string_body> req,
    http::response<http::dynamic_body> res);
