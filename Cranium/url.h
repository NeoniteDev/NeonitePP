#pragma once
#include <algorithm>
#include <string>
#include <sstream>
#include <string_view>

struct Uri
{
private:
	typedef std::string_view::const_iterator iterator_t;

public:
	std::string_view Protocol, Host, Port, Path, QueryString;

	static Uri Parse(const std::string_view& uri)
	{
		Uri result;

		if (uri.length() == 0) return result;

		iterator_t uriEnd = uri.end();

		iterator_t queryStart = std::find(uri.begin(), uriEnd, '?');

		iterator_t protocolStart = uri.begin();
		iterator_t protocolEnd = std::find(protocolStart, uriEnd, ':');

		if (protocolEnd != uriEnd)
		{
			std::string_view prot = &*(protocolEnd);
			if ((prot.length() > 3) && (prot.substr(0, 3) == "://"))
			{
				result.Protocol = make_string_view(uri, protocolStart, protocolEnd);
				protocolEnd += 3;
			}
			else protocolEnd = uri.begin();
		}
		else protocolEnd = uri.begin();

		iterator_t hostStart = protocolEnd;
		iterator_t pathStart = std::find(hostStart, uriEnd, '/');

		iterator_t hostEnd = std::find(protocolEnd, (pathStart != uriEnd) ? pathStart : queryStart, ':');

		result.Host = make_string_view(uri, hostStart, hostEnd);

		if ((hostEnd != uriEnd) && ((&*(hostEnd))[0] == ':'))
		{
			++hostEnd;
			iterator_t portEnd = (pathStart != uriEnd) ? pathStart : queryStart;
			result.Port = make_string_view(uri, hostEnd, portEnd);
		}

		if (pathStart != uriEnd) result.Path = make_string_view(uri, pathStart, queryStart);

		if (queryStart != uriEnd) result.QueryString = make_string_view(uri, queryStart, uri.end());

		return result;
	}

	static std::string CreateUri(std::string_view Protocol, std::string_view Host, std::string_view Port, std::string_view Path, std::string_view QueryString)
	{
		std::ostringstream str;
		if (!Protocol.empty())
		{
			str.write(Protocol.data(), Protocol.size());
			str.write("://", 3);
		}
		str.write(Host.data(), Host.size());
		if (!Port.empty())
		{
			str.write(":", 1);
			str.write(Port.data(), Port.size());
		}
		if (!Path.empty())
		{
			str.write(Path.data(), Path.size());
		}
		if (!QueryString.empty())
		{
			str.write(QueryString.data(), QueryString.size());
		}
		return str.str();
	}

private:
	static constexpr std::string_view make_string_view(const std::string_view& base, iterator_t first, iterator_t last)
	{
		return base.substr(std::distance(base.begin(), first), std::distance(first, last));
	}
};
