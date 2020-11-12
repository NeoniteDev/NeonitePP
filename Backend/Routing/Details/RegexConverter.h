#pragma once

#include <string>

namespace Routing {
	namespace Details {
		class RegexConverter {
		public:
			virtual std::string toRegex(std::string const& path) const;
		};
	}
}
