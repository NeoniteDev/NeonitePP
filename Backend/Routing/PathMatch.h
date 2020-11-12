#pragma once

#include <string>
#include <map>

#include "Details/PathTemplate.h"

namespace Routing {
	class PathMatch final {
	public:
		friend class Router;

		/**
		 * @brief Get actual path (e.g /streams/1)
		 */
		std::string path() const { return _path; }

		/**
		 * @brief Get path template (e.g. /some/path or /some/:dynamic/path)
		 * @returns path template
		 */
		std::string pathTemplate() const;

		/**
		 * @brief Get dynamic variable
		 * @param variable name
		 * @returns variable
		 * @throws NoSuchElementException if path template doesn't contain variable with given name
		 * @details example: for path template [/path/to/:var] and actual
		 * path [/path/to/123] result will be '123'
		 */
		std::string getVar(std::string const& name) const;

		/**
		 * @brief Get dynamic variable
		 * @param variable name
		 * @returns variable
		 * @throws NoSuchElementException if path template doesn't contain variable with given name
		 * @details example: for path template [/path/to/:var] and actual
		 * path [/path/to/123] result will be '123'
		 */
		std::string operator[](std::string const& name) const;

	protected:
		PathMatch(std::string path, Details::PathTemplate const& tpl);

	private:
		std::string _path;
		Details::StringTokenizer _tokenizer;
		Details::PathTemplate _pathTemplate;
	};
}
