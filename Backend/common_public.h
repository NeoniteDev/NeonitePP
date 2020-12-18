#include "../Frontend/util.h"

inline json profile_public =
R"(
{
  "created": "",
  "updated": "",
  "rvn": 1,
  "wipeNumber": 1,
  "accountId": "",
  "profileId": "common_public",
  "version": "neonite++",
  "items": {},
  "stats": {
    "attributes": {
      "banner_color": "",
      "homebase_name": "",
      "banner_icon": ""
    }
  }
}
)"_json;

inline json pCommonPublic()
{
    boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
    auto date = to_iso_extended_string(t);
    date = date.substr(0, date.size() - 3) + "Z";
    profile_public["created"] = date;
    profile_public["updated"] = date;
    profile_public["accountId"] = name;
    return profile_public;
}
