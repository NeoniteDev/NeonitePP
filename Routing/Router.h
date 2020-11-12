#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>

#include "PathMatch.h"
#include "Details/RegexConverter.h"

namespace Routing {

namespace Details {
class PathTemplate;
}

/**
 * @brief Simple uri router
 */
class Router {
public:
    Router();

    /**
    * @brief register path
    * @param path path (e.g. /routes/path, /routes/:some_var)
    * @returns path id
    */
    void registerPath(std::string const &path);

    /**
    * @brief match path
    * @param path (e.g. /routes/path)
    * @returns path id if matched, or -1 if no match detected
    */
    PathMatch matchPath(std::string const &path);

private:
    std::vector<Details::PathTemplate> _templates;
    std::unique_ptr<Details::RegexConverter> _regexConverter;
};

}