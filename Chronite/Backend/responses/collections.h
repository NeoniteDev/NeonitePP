#include "../Frontend/util.h"

inline json profile_collections =
R"(
{
    "created": "",
    "updated": "",
    "rvn": 1,
    "wipeNumber": 1,
    "accountId": "",
    "profileId": "collections",
    "version": "neonite++",
    "items": {},
    "stats": {
      "attributes": {}
    }
  }
)"_json;

inline json pCollections()
{
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	auto date = to_iso_extended_string(t);
	date = date.substr(0, date.size() - 3) + "Z";
	profile_collections["created"] = date;
	profile_collections["updated"] = date;
	profile_collections["accountId"] = name;
	return profile_collections;
}
