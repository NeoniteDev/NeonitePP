#pragma once

#include <vector>
#include <string>

namespace Routing {
namespace Details {


/**
 * @brief Simple string tokenizer. Works only with single character delimeter.
 */
class StringTokenizer {
public:
    struct TokenPos {
        int first;
        int len;
    };

    StringTokenizer(std::string const &str, char delim);

    int numTokens() const;

    std::vector<std::string> getAllTokens() const;

    std::string getToken(int index) const;
    TokenPos getTokenPos(int index) const;

private:
    std::string _origin;
    std::vector<TokenPos> _tokens;
    char _delimeter;

    void tokenize();
    int nextDelim(std::string const &str, int startPos);
};

}
}