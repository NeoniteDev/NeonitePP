#pragma once

//#define LOGGING

#include "../includes/framework.h"
#include "server/athena.h"
#include "server/collections.h"
#include "server/common_core.h"
#include "server/common_public.h"
#include "server/creative.h"
#include "server/calendar.h"
#include "server/fortnite-game.h"

using namespace httplib;

inline HANDLE hServer = INVALID_HANDLE_VALUE;
inline Server app;

#include "server/statics.h"
#include "server/api.h"
#include "server/dynamics.h"
#include "server/profile.h"

namespace server
{
	inline void init()
	{
		initStatics();
		initApi();
		initDynamics();
		initProfile();

		app.set_error_handler([](const auto& req, auto& res)
		{
			auto fmt = "{}";
			char buf[BUFSIZ];
			snprintf(buf, sizeof(buf), fmt, res.status);
			auto test = req.path;
			res.set_content(buf, "application/json");
		});

#ifdef LOGGING
		app.set_logger([](const Request& req, const Response& res)
		{
			std::string log = "Request: " + req.method + "\nPath: " + req.path + "\nBody: " + req.body + "\nResponse: " + res.body;
			console.AddLog(log.c_str());
		});
#endif

		app.listen("localhost", 5595);
	}
}
