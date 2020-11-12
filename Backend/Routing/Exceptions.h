#pragma once

#include <system_error>
#include <string>

namespace Routing {
	class RoutingException : public std::runtime_error {
	public:
		RoutingException(std::string const& what)
			: std::runtime_error(what) {
		}
	};

	class NoSuchElementException : public RoutingException {
	public:
		NoSuchElementException(std::string const& what)
			: RoutingException(what) {
		}
	};

	class OutOfRangeException : public RoutingException {
	public:
		OutOfRangeException(std::string const& what)
			: RoutingException(what) {
		}
	};

	class InvalidPathException : public RoutingException {
	public:
		InvalidPathException(std::string const& what)
			: RoutingException(what) {
		}
	};

	class PathNotFoundException : public RoutingException {
	public:
		PathNotFoundException(std::string const& what)
			: RoutingException(what) {
		}
	};
}
