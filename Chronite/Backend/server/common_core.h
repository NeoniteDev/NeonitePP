#pragma once

inline json profile_core =
R"(
{
  "_id": "",
  "created": "",
  "updated": "",
  "rvn": 1,
  "wipeNumber": 1,
  "accountId": "",
  "profileId": "common_core",
  "version": "neonitepp",
  "items": {},
  "stats": {
    "attributes": {
      "mtx_affiliate": "neonite++",
      "current_mtx_platform": "EpicPC"
  }
 }
}
)"_json;

inline json pCommonCore()
{
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	auto date = to_iso_extended_string(t);
	date = date.substr(0, date.size() - 3) + "Z";
	profile_core["created"] = date;
	profile_core["updated"] = date;
	profile_core["_id"] = name;
	profile_core["accountId"] = name;
	return profile_core;
}
