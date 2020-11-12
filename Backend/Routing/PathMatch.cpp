#include "PathMatch.h"
#include "Exceptions.h"

namespace Routing {
	PathMatch::PathMatch(std::string path, Details::PathTemplate const& tpl)
		: _path(path), _pathTemplate(tpl), _tokenizer(path, '/') {
	}

	std::string PathMatch::getVar(std::string const& name) const {
		if (!_pathTemplate.parser().hasDynVar(name)) {
			throw NoSuchElementException(
				"Path [" + _path + "] with template [" + _pathTemplate.templ() + "]" +
				"does not contain variable '" + name + "'");
		}

		int pos = _pathTemplate.parser().getDynVarPos(name);
		return _tokenizer.getToken(pos);
	}

	std::string PathMatch::pathTemplate() const {
		return _pathTemplate.templ();
	}

	std::string PathMatch::operator[](std::string const& name) const {
		return getVar(name);
	}
}