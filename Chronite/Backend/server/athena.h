#pragma once

inline json profile_athena =
R"(
{
  "_id": "",
  "created": "",
  "updated": "",
  "rvn": 1,
  "wipeNumber": 1,
  "accountId": "",
  "profileId": "athena",
  "version": "neonite++",
  "items": {
    "sandbox_loadout": {
      "templateId": "CosmeticLocker:cosmeticlocker_athena",
      "attributes": {
        "locker_slots_data": {
          "slots": {
            "Pickaxe": {
              "items": [
                "AthenaPickaxe:pickaxe_id_420_candyapplesour_jxbza"
              ],
              "activeVariants": []
            },
            "Dance": {
              "items": [
                "AthenaDance:eid_loofah",
                null,
                null,
                null,
                null,
                null
              ]
            },
            "Glider": {
              "items": [
                null
              ]
            },
            "Character": {
              "items": [
                "AthenaCharacter:cid_791_athena_commando_m_henchmangoodshorts_d"
              ],
              "activeVariants": [
                {
                  "variants": [
                    {
                      "channel": "Parts",
                      "active": "Stage2",
                      "owned": []
                    },
                    {
                      "channel": "Material",
                      "active": "Mat2",
                      "owned": []
                    }
                  ]
                }
              ]
            },
            "Backpack": {
              "items": [
                "AthenaBackpack:bid_208_cupiddarkmale"
              ],
              "activeVariants": []
            },
            "ItemWrap": {
              "items": [
                null,
                null,
                null,
                null,
                null,
                null,
                null
              ],
              "activeVariants": [
                null,
                null,
                null,
                null,
                null,
                null,
                null
              ]
            },
            "LoadingScreen": {
              "items": [
                null
              ],
              "activeVariants": [
                null
              ]
            },
            "MusicPack": {
              "items": [
                null
              ],
              "activeVariants": [
                null
              ]
            },
            "SkyDiveContrail": {
              "items": [
                null
              ],
              "activeVariants": [
                null
              ]
            }
          }
        },
        "use_count": 0,
        "banner_icon_template": "BRS1080sLlama",
        "locker_name": "",
        "banner_color_template": "DefaultColor1",
        "item_seen": false,
        "favorite": false
      },
      "quantity": 1
    }
  },
  "stats": {
    "attributes": {
      "season_match_boost": 999,
      "loadouts": [
        "sandbox_loadout",
        "loadout_1"
      ],
      "rested_xp_overflow": 0,
      "mfa_reward_claimed": true,
      "quest_manager": {},
      "book_level": 1000,
      "season_num": 12,
      "season_update": 0,
      "book_xp": 0,
      "permissions": [],
      "book_purchased": true,
      "lifetime_wins": 999,
      "party_assist_quest": "",
      "purchased_battle_pass_tier_offers": [],
      "rested_xp_exchange": 0.333,
      "level": 1000,
      "xp_overflow": 0,
      "rested_xp": 204000,
      "rested_xp_mult": 12.75,
      "accountLevel": 10000,
      "competitive_identity": {},
      "inventory_limit_bonus": 0,
      "last_applied_loadout": "loadout_1",
      "daily_rewards": {},
      "xp": 0,
      "season_friend_match_boost": 0,
      "active_loadout_index": 0
    }
  }
}
)"_json;

inline json pAthena()
{
	boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
	auto date = to_iso_extended_string(t);
	date = date.substr(0, date.size() - 3) + "Z";
	profile_athena["created"] = date;
	profile_athena["updated"] = date;
	profile_athena["_id"] = name;
	profile_athena["accountId"] = name;
    profile_athena["stats"]["attributes"]["book_level"] = seasonLevel;
    profile_athena["stats"]["attributes"]["book_purchased"] = bHasBattlepass;
	return profile_athena;
}
