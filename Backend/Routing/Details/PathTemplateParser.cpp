#include "PathTemplateParser.h"
#include "../Exceptions.h"

#include <regex>

namespace Routing {
namespace Details {

PathTemplateParser::PathTemplateParser(std::string const &path)
  : _tokenizer(path, '/') {
  if (!validate(path)) {
    throw InvalidPathException("PathTemplate [" + path + "] is invalid");
  }

  parse(path);
}

void PathTemplateParser::parse(std::string const &path) {
  for (int i = 0; i < _tokenizer.numTokens(); i++) {
    std::string token = _tokenizer.getToken(i);
    if (token[0] == ':') {
      _dynVarsPos[token.substr(1, token.length())] = i;
    }
  }
}

bool PathTemplateParser::validate(std::string const &path) {
  return std::regex_match(path, std::regex("(\\/[^\\/]+)+\\/?"));
}

int PathTemplateParser::getDynVarPos(std::string const &name) const {
  auto it = _dynVarsPos.find(name);

  if (it == _dynVarsPos.end()) {
    throw NoSuchElementException(
        "PathTemplate 'does not contain variable '" + name + "'");
  }

  return it->second;
}

bool PathTemplateParser::hasDynVar(std::string const &name) const {
  return _dynVarsPos.find(name) != _dynVarsPos.end();
}

bool PathTemplateParser::hasDynVars() const {
  return _dynVarsPos.size() > 0;
}

}
}
