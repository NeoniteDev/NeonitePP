#include "../Frontend/util.h"

json profile_creative =
R"(
{
    "created": "",
    "updated": "",
    "rvn": 1,
    "wipeNumber": 1,
    "accountId": "",
    "profileId": "creative",
    "version": "neonite++",
    "items": {},
    "stats": {
      "attributes": {
        "last_used_battlelab_file": "",
        "max_island_plots": 50,
        "publish_allowed": true,
        "support_code": "",
        "last_used_plot": "",
        "permissions": [],
        "creator_name": "",
        "publish_banned": false,
        "inventory_limit_bonus": 0
      }
    }
  }
)"_json;

inline json pCreative()
{
    boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
    auto date = to_iso_extended_string(t);
    date = date.substr(0, date.size() - 3) + "Z";
    profile_creative["created"] = date;
    profile_creative["updated"] = date;
    profile_creative["accountId"] = name;
    return profile_creative;
}
