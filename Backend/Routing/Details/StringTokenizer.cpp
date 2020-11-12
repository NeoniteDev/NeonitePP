#include "StringTokenizer.h"

#include "../Exceptions.h"

namespace Routing {
	namespace Details {
		StringTokenizer::StringTokenizer(std::string const& str, char delim)
			: _origin(str), _delimeter(delim) {
			tokenize();
		}

		void StringTokenizer::tokenize() {
			int startPos = _origin[0] == _delimeter ? 1 : 0;
			int tokenEndPos = startPos;

			while (startPos <= _origin.length()) {
				tokenEndPos = nextDelim(_origin, startPos);
				_tokens.push_back(TokenPos{ startPos, tokenEndPos - startPos });
				startPos = tokenEndPos + 1;
			}

			if (_tokens.back().len == 0) {
				_tokens.erase(_tokens.end() - 1);
			}
		}

		int StringTokenizer::nextDelim(std::string const& str, int startPos) {
			int res;
			for (res = startPos; res < str.length() && str[res] != _delimeter; res++);
			return res;
		}

		std::vector<std::string> StringTokenizer::getAllTokens() const {
			std::vector<std::string> result;
			for (auto token : _tokens) {
				result.push_back(_origin.substr(token.first, token.len));
			}
			return result;
		}

		std::string StringTokenizer::getToken(int index) const {
			if (index >= _tokens.size()) {
				throw OutOfRangeException(
					"StringTokenizer::getToken: Index " + std::to_string(index) +
					" was out of range (" + std::to_string(_tokens.size()) + ")");
			}

			return _origin.substr(_tokens[index].first, _tokens[index].len);
		}

		StringTokenizer::TokenPos StringTokenizer::getTokenPos(int index) const {
			if (index >= _tokens.size()) {
				throw OutOfRangeException(
					"StringTokenizer::getTokenPos: Index " + std::to_string(index) +
					" was out of range (" + std::to_string(_tokens.size()) + ")");
			}

			return _tokens[index];
		}

		int StringTokenizer::numTokens() const {
			return _tokens.size();
		}
	}
}