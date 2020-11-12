#include "RegexConverter.h"

#include <regex>

namespace Routing {
namespace Details {

std::string RegexConverter::toRegex(std::string const &path) const {
  std::string slashes = std::regex_replace(path, std::regex("\\/"), "\\/");
  std::string dynvars = std::regex_replace(slashes, std::regex(":[^\\/\\\\]+"),
                                           "[^\\/]+");
  return dynvars;
}

}
}
