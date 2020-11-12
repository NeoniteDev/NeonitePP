#pragma once

#include <regex>
#include <string>

#include "StringTokenizer.h"
#include "PathTemplateParser.h"
#include "RegexConverter.h"

namespace Routing {
namespace Details {

class PathTemplate {
public:
  PathTemplate(std::string const &templ);

  std::string const &templ() const { return _templ; }

  std::regex const &regex() const { return _regex; }

  PathTemplateParser &parser() { return _parser; }
  PathTemplateParser const &parser() const { return _parser; }

private:
  std::regex _regex;
  PathTemplateParser _parser;
  std::string _templ;
};

}
}
