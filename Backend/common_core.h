#include "../Frontend/util.h"

json profile =
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
  "items": {
    "Currency:MtxPurchased": {
      "attributes": {
        "platform": "EpicPC"
      },
      "quantity": 9999999,
      "templateId": "Currency:MtxPurchased"
    },
    "HomebaseBannerIcon:standardbanner1": {
      "templateId": "HomebaseBannerIcon:standardbanner1",
      "attributes": {
        "item_seen": false
      },
      "quantity": 1
    }
  },
  "stats": {
    "attributes": {
      "mtx_affiliate": "neonite++",
      "current_mtx_platform": "EpicPC",
      "mtx_purchase_history": {
        "refundsUsed": 0,
        "refundCredits": 3,
        "purchases": [{
            "purchaseId": "c757f73c-ee23-48d2-a8ac-0cdb4b740f4e",
            "offerId": "v2:/7b13321c66646d279cdff93b48b5c20d735b66f221e4e53231abfc91a3ab6160",
            "purchaseDate": "9999-12-30T20:39:54.252Z",
            "freeRefundEligible": false,
            "fulfillments": [],
            "lootResult": [{
                "itemType": "AthenaCharacter:CID_902_Athena_Commando_M_PumpkinPunk",
                "itemGuid": "AthenaCharacter:CID_902_Athena_Commando_M_PumpkinPunk",
                "itemProfile": "athena",
                "quantity": 1
            }],
            "totalMtxPaid": 1500,
            "metadata": {},
            "gameContext": ""
        }]
    }
    }
  }
}
)"_json;

inline json common_core()
{
    boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
    auto date = to_iso_extended_string(t);
    profile["created"] = date;
    profile["updated"] = date;
    profile["_id"] = name;
    profile["accountId"] = name;
    profile["items"]["Currency:MtxPurchased"]["quantity"] = vbucks;
    return profile;
}
