#pragma once

#include <map>
#include <string>
#include <vector>

#include "StringTokenizer.h"

namespace Routing {
namespace Details {

class PathTemplateParser {
public:
    PathTemplateParser(std::string const &path);

    bool hasDynVars() const;
    bool hasDynVar(std::string const &name) const;
    int getDynVarPos(std::string const &name) const;

private:
    std::map<std::string, int> _dynVarsPos;
    StringTokenizer _tokenizer;

    void parse(std::string const &path);
    bool validate(std::string const &path);
};

}
}
