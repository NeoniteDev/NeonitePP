#include "PathTemplate.h"

namespace Routing {
	namespace Details {
		PathTemplate::PathTemplate(std::string const& path)
			: _parser(path), _templ(path) {
			_regex = RegexConverter().toRegex(path);
		}
	}
}