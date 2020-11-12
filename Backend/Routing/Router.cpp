#include "Router.h"

#include "Details/PathTemplate.h"
#include "Details/RegexConverter.h"
#include "Exceptions.h"

namespace Routing {
	using Details::PathTemplate;
	using Details::RegexConverter;

	Router::Router() {
	}

	void Router::registerPath(std::string const& path) {
		_templates.push_back(PathTemplate(path));
	}

	PathMatch Router::matchPath(std::string const& path) {
		for (auto& tpl : _templates) {
			if (std::regex_match(path, tpl.regex())) {
				return PathMatch(path, tpl);
			}
		}

		throw PathNotFoundException("Path not found for '" + path + "'");
	}
}