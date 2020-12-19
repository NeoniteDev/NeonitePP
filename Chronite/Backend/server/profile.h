#pragma once

inline void initProfile()
{
	app.Post(R"(/fortnite/api/game/v2/profile/(.*)/client/(.*))", [](const Request& req, Response& res)
	         {
		         //TODO: handle this somewhere else.
		         std::string account_id = static_cast<std::string>(req.matches[1]);
		         std::string command = static_cast<std::string>(req.matches[2]);
		         std::string profile_id = "common_core";
		         int rvn = -1;
		         boost::posix_time::ptime t = boost::posix_time::microsec_clock::universal_time();
		         auto date = to_iso_extended_string(t);
		         date = date.substr(0, date.size() - 3) + "Z";
		         json profileData;

		         if (req.has_param("rvn")) rvn = std::stoi(req.get_param_value("rvn"));
		         if (req.has_param("profileId")) profile_id = req.get_param_value("profileId");

		         if (profile_id == "athena") profileData = pAthena();
		         else if (profile_id == "common_public") profileData = pCommonPublic();
		         else if (profile_id == "creative") profileData = pCreative();
		         else if (profile_id == "collections") profileData = pCollections();
		         else profileData = pCommonCore();

		         if (profileData["rvn"].is_null()) profileData["rvn"] = 1;
		         if (profileData["commandRevision"].is_null()) profileData["commandRevision"] = 1;

		         json response = {
			         {"profileRevision", profileData["rvn"]},
			         {"profileId", profile_id},
			         {"profileChangesBaseRevision", profileData["rvn"]},
			         {"profileChanges", json::array()},
			         {"serverTime", date},
			         {"profileCommandRevision", profileData["commandRevision"]},
			         {"responseVersion", 1}
		         };

		         //mcp

		         switch (util::str2int(command.c_str()))
		         {
		         case util::str2int("ClientQuestLogin"):
		         {
			         break;
		         }
		         case util::str2int("SetCosmeticLockerSlot"):
		         {
			         auto body = json::parse(req.body);
			         auto lockerItem = body["lockerItem"].get<std::string>();
			         auto items = profileData["items"];
			         auto item = items[lockerItem];
			         if (item.is_null())
			         {
				         //TODO: add mpc error handling.
			         }
			         auto category = body["category"].get<std::string>();
			         auto locker_slot_data = item["attributes"]["locker_slots_data"];
			         auto lockerSlot = locker_slot_data["slots"][category];
			         if (lockerSlot.is_null())
			         {
				         //Emotes and wraps
			         }
			         auto itemsArray = lockerSlot["items"];
			         bool bChanged = false;

			         auto slotIndex = body["slotIndex"].get<int>();
			         if (!slotIndex) slotIndex = 0;

			         if (slotIndex == -1)
			         {
				         //Apply to all
			         }
			         else
			         {
				         if (slotIndex >= 0 && itemsArray.size())
				         {
					         if (itemsArray[slotIndex] != body["itemToSlot"])
					         {
						         itemsArray[slotIndex] = body["itemToSlot"];
						         bChanged = true;
					         }
				         }
				         profileData["items"][lockerItem]["attributes"]["locker_slots_data"]["slots"][category]["items"] = itemsArray;
			         }

			         if (body["variantUpdates"].size() != 0)
			         {
				         //variants update
			         }

			         if (bChanged)
			         {
				         //TODO: make this a function.

				         auto itemId = body["lockerItem"].get<std::string>();
				         auto item = profileData["items"][itemId];
				         //if (item.is_null()) return false;
				         if (item["attributes"].is_null())
				         {
					         item["attributes"] = json::object();
				         }
				         item["attributes"]["locker_slots_data"] = locker_slot_data;
				         json changes = {
					         {"changeType", "itemAttrChanged"},
					         {"itemId", itemId},
					         {"attributeName", "locker_slots_data"},
					         {"attributeValue", locker_slot_data}
				         };
				         response["profileChanges"].push_back(changes);
			         }

			         break;
		         }
		         case util::str2int("MarkItemSeen"):
		         {
			         //req.body.itemIds.forEach(itemId => Profile.changeItemAttribute(profileData, itemId, "item_seen", true, profileChanges));
			         /*
			          json itemsIds = json::parse(req.body)["itemsIds"];
			          for (auto& item : itemsIds.items())
			          {
				          std::string itemId = item.value();
				          response["profileChanges"].push_back({
					          {"changeType", "itemRemoved"},
					          {"itemId", itemId}
				          });
			          }
			          */
			         break;
		         }
		         }

		         if (response["profileChanges"].size() > 0)
		         {
			         profileData["rvn"] = profileData["rvn"].get<int>() + 1;
			         profileData["commandRevision"] = profileData["commandRevision"].get<int>() + 1;
			         profileData["updated"] = date;
			         response["profileRevision"] = profileData["rvn"];
			         response["profileCommandRevision"] = profileData["commandRevision"];

			         if (profile_id == "athena") profile_athena = profileData;
			         else if (profile_id == "common_public") profile_public = profileData;
			         else if (profile_id == "creative") profile_creative = profileData;
			         else if (profile_id == "collections") profile_collections = profileData;
			         else profileData = profile_core = profileData;
		         }

		         if (rvn != response["profileChangesBaseRevision"].get<int>())
		         {
			         response["profileChanges"][0] = {
				         {"changeType", "fullProfileUpdate"},
				         {"profile", profileData}
			         };
		         }

		         res.set_content(response.dump(), "application/json");
	         }
	);
}