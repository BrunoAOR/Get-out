#include "GameData.h"

#include <fstream>
#include "globals.h"
#include "ActionEffectInfo.h"
#include "ActionEffectType.h"
#include "AddEntitiesInfoIO.h"
#include "GameEndInfoIO.h"
#include "LockExitInfoIO.h"
#include "PlaceItemInItemInfoIO.h"
#include "RemoveEntitiesInfoIO.h"
#include "ReplaceEntityInfoIO.h"
#include "UnlockExitInfoIO.h"
#include "ActionInfo.h"
#include "ActionType.h"
#include "Direction.h"
#include "EntityInfo.h"
#include "EntityType.h"
#include "json.hpp"


// Json
namespace
{
	using Json = nlohmann::json;


	Json loadJson(const char* aPath)
	{
		Json lJson;

		std::ifstream lFile(aPath);
		if (lFile.good())
		{
			lFile >> lJson;
		}
		return lJson;
	}
}// anonymous


// Messages loading
namespace
{
	bool loadMessages(const Json& aJson, std::string& aOutWelcomeMessageString, std::string& aOutExitMessageString)
	{
		bool lSuccess = true;

		if (aJson.count("messages"))
		{
			const Json& messages = aJson["messages"];
			if (lSuccess && messages.count("welcomeMessage"))
			{
				aOutWelcomeMessageString = (messages["welcomeMessage"]).get<std::string>();
			}
			else
			{
				OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'welcomeMessage' within 'messages'!");
				lSuccess = false;
			}

			if (lSuccess && messages.count("exitMessage"))
			{
				aOutExitMessageString = (messages["exitMessage"]).get<std::string>();
			}
			else
			{
				OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'exitMessage' within 'messages'!");
				lSuccess = false;
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'messages'!");
			lSuccess = false;
		}
		return lSuccess;
	}
}// anonymous


// Entity loading
namespace
{
	using entityLoaderFunc = bool(const Json&, std::vector< EntityInfo >&);


	bool loadPlayerInfo(const Json& aJsonPlayer, std::vector< EntityInfo >& aInOutEntityInfos)
	{
		bool lSuccess = true;

		if (aJsonPlayer.count("id") && aJsonPlayer.count("parentId") && aJsonPlayer.count("maxItems") && aJsonPlayer.count("startingRoomId"))
		{
			EntityInfo lPlayerInfo = EntityInfo::createPlayerInfo(aJsonPlayer["id"], EntityType::PLAYER, aJsonPlayer["parentId"], "", "", aJsonPlayer["maxItems"], aJsonPlayer["startingRoomId"]);
			aInOutEntityInfos.push_back(lPlayerInfo);
		}
		else
		{
			OUTPUT_LOG("ERROR: Player doesn't have all the required keys!");
			lSuccess = false;
		}

		return lSuccess;
	}


	bool loadRoomInfos(const Json& aJsonRooms, std::vector< EntityInfo >& aInOutEntityInfos)
	{
		bool lSuccess = true;

		for (unsigned int i = 0, lRoomsCount = aJsonRooms.size(); i < lRoomsCount; ++i)
		{
			const Json& lRoom = aJsonRooms[i];
			if (lRoom.count("id") && lRoom.count("name") && lRoom.count("description") && lRoom.count("isDark"))
			{
				EntityInfo lEntityInfo = EntityInfo::createRoomInfo(lRoom["id"], EntityType::ROOM, -1, lRoom["name"], lRoom["description"], lRoom["isDark"]);
				aInOutEntityInfos.push_back(lEntityInfo);
			}
			else
			{
				OUTPUT_LOG("ERROR: Room at index %i doesn't have all the required keys!", i);
				lSuccess = false;
			}
		}

		return lSuccess;
	}


	bool loadExitInfos(const Json& aJsonExits, std::vector< EntityInfo >& aInOutEntityInfos)
	{
		bool lSuccess = true;

		for (unsigned int i = 0, exitsCount = aJsonExits.size(); i < exitsCount; ++i)
		{
			const Json& lExit = aJsonExits[i];
			if (lExit.count("id") && lExit.count("parentId") && lExit.count("name") && lExit.count("description") && lExit.count("direction") && lExit.count("isLocked") && lExit.count("lockedDescription") && lExit.count("targetRoomId"))
			{
				if (getDirectionFromString(lExit["direction"]) != Direction::_UNDEFINED)
				{
					EntityInfo lEntityInfo = EntityInfo::createExitInfo(lExit["id"], EntityType::EXIT, lExit["parentId"], lExit["name"], lExit["description"], getDirectionFromString(lExit["direction"]), lExit["isLocked"], lExit["lockedDescription"], lExit["targetRoomId"]);
					aInOutEntityInfos.push_back(lEntityInfo);
				}
				else
				{
					OUTPUT_LOG("ERROR: Exit at index %i doesn't have a valid value for the key 'direction'!", i);
				}
			}
			else
			{
				OUTPUT_LOG("ERROR: Exit at index %i doesn't have all the required keys!", i);
				lSuccess = false;
			}
		}

		return lSuccess;
	}


	bool loadInteractableInfos(const Json& aJsonInteractables, std::vector< EntityInfo >& aInOutEntityInfos)
	{
		bool lSuccess = true;

		for (unsigned int i = 0, lInteractablesCount = aJsonInteractables.size(); i < lInteractablesCount; ++i)
		{
			const Json& lInteractable = aJsonInteractables[i];
			if (lInteractable.count("id") && lInteractable.count("parentId") && lInteractable.count("name") && lInteractable.count("description") && lInteractable.count("inspectDescription") && lInteractable.count("isVisibleInDark"))
			{
				EntityInfo lEntityInfo = EntityInfo::createInteractableInfo(lInteractable["id"], EntityType::INTERACTABLE, lInteractable["parentId"], lInteractable["name"], lInteractable["description"], lInteractable["inspectDescription"], lInteractable["isVisibleInDark"]);
				aInOutEntityInfos.push_back(lEntityInfo);
			}
			else
			{
				OUTPUT_LOG("ERROR: Interactable at index %i doesn't have all the required keys!", i);
				lSuccess = false;
			}
		}

		return lSuccess;
	}


	bool loadItemInfos(const Json& aJsonItems, std::vector< EntityInfo >& aInOutEntityInfos)
	{
		bool lSuccess = true;

		for (unsigned int i = 0, lItemsCount = aJsonItems.size(); i < lItemsCount; ++i)
		{
			const Json& lItem = aJsonItems[i];
			if (lItem.count("id") && lItem.count("parentId") && lItem.count("name") && lItem.count("description") && lItem.count("inspectDescription") && lItem.count("isVisibleInDark") && lItem.count("hasLight"))
			{
				EntityInfo lEntityInfo = EntityInfo::createItemInfo(lItem["id"], EntityType::ITEM, lItem["parentId"], lItem["name"], lItem["description"], lItem["inspectDescription"], lItem["isVisibleInDark"], lItem["hasLight"]);
				aInOutEntityInfos.push_back(lEntityInfo);
			}
			else
			{
				OUTPUT_LOG("ERROR: Item at index %i doesn't have all the required keys!", i);
				lSuccess = false;
			}
		}

		return lSuccess;
	}


	bool loadEntitiesByKey(const Json& aJsonEntityInfos, const std::string& aKey, entityLoaderFunc aLoaderFunc, std::vector< EntityInfo >& aInOutEntityInfos)
	{
		bool lSuccess = true;

		if (aJsonEntityInfos.count(aKey))
		{
			const Json& lJsonRooms = aJsonEntityInfos[aKey];
			if (!(aLoaderFunc(lJsonRooms, aInOutEntityInfos)))
			{
				OUTPUT_LOG("ERROR: Failed to load rooms from the game configuration file!");
				lSuccess = false;
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: The gameConfig file does not contain the key '%s' within 'entityInfos'!", aKey.c_str());
			lSuccess = false;
		}

		return lSuccess;
	}


	bool loadEntities(const Json& aJson, EntityInfo& aOutPlayerInfo, std::vector< EntityInfo >& aOutEntityInfos)
	{
		bool lSuccess = true;

		if (aJson.count("entityInfos"))
		{
			const Json& lJsonEntityInfos = aJson["entityInfos"];

			bool lSuccess = loadEntitiesByKey(lJsonEntityInfos, "rooms", loadRoomInfos, aOutEntityInfos)
				&& loadEntitiesByKey(lJsonEntityInfos, "exits", loadExitInfos, aOutEntityInfos)
				&& loadEntitiesByKey(lJsonEntityInfos, "interactables", loadInteractableInfos, aOutEntityInfos)
				&& loadEntitiesByKey(lJsonEntityInfos, "items", loadItemInfos, aOutEntityInfos)
				&& loadEntitiesByKey(lJsonEntityInfos, "player", loadPlayerInfo, aOutEntityInfos);

			aOutPlayerInfo = aOutEntityInfos.back();
			aOutEntityInfos.pop_back();
		}
		else
		{
			OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'entityInfos'!");
			lSuccess = false;
		}

		return lSuccess;
	}
}// anonymous


// Action loading
namespace
{
	ActionEffectInfo loadActionEffect(const Json& aJsonEffect, ActionEffectType aEffectType, int aActionIndex, int aEffectIndex)
	{
		ASSERT(aEffectType != ActionEffectType::_UNDEFINED);

		ActionEffectInfo lEffectInfo;

		// Now we can switch over aEffectType and load them appropriately
		switch (aEffectType)
		{

		case ActionEffectType::ADD_ENTITIES:
		{
			if (aJsonEffect.count("entitiesToAddIds") && aJsonEffect.count("targetRoomId"))
			{
				lEffectInfo = AddEntitiesInfoIO::createInfo(aJsonEffect["description"], aJsonEffect["targetRoomId"], aJsonEffect["entitiesToAddIds"].begin(), aJsonEffect["entitiesToAddIds"].end());
			}
			break;
		}

		case ActionEffectType::GAME_END:
		{
			lEffectInfo = GameEndInfoIO::createInfo(aJsonEffect["description"]);
			break;
		}

		case ActionEffectType::LOCK_EXIT:
		{
			if (aJsonEffect.count("exitToLockId"))
			{
				lEffectInfo = LockExitInfoIO::createInfo(aJsonEffect["description"], aJsonEffect["exitToLockId"]);
			}
			break;
		}

		case ActionEffectType::PLACE_ITEM_IN_ITEM:
		{
			if (aJsonEffect.count("itemId") && aJsonEffect.count("containerId"))
			{
				lEffectInfo = PlaceItemInItemInfoIO::createInfo(aJsonEffect["description"], aJsonEffect["itemId"], aJsonEffect["containerId"]);
			}
			break;
		}

		case ActionEffectType::REMOVE_ENTITIES:
		{
			if (aJsonEffect.count("entitiesToRemoveIds"))
			{
				lEffectInfo = RemoveEntitiesInfoIO::createInfo(aJsonEffect["description"], aJsonEffect["entitiesToRemoveIds"].begin(), aJsonEffect["entitiesToRemoveIds"].end());
			}
			break;
		}

		case ActionEffectType::REPLACE_ENTITY:
		{
			if (aJsonEffect.count("entityToRemoveId") && aJsonEffect.count("entityToAddId"))
			{
				lEffectInfo = ReplceEntityInfoIO::createInfo(aJsonEffect["description"], aJsonEffect["entityToRemoveId"], aJsonEffect["entityToAddId"]);
			}
			break;
		}

		case ActionEffectType::UNLOCK_EXIT:
		{
			if (aJsonEffect.count("exitToUnlockId"))
			{
				lEffectInfo = UnlockExitInfoIO::createInfo(aJsonEffect["description"], aJsonEffect["exitToUnlockId"]);
			}
			break;
		}

		}// switch (aEffectType)

		if (!lEffectInfo.isValid())
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}

		return lEffectInfo;
	}


	bool loadActionEffects(const Json& aJsonEffects, int aActionIndex, std::vector< ActionEffectInfo >& aInOutActionEffectInfos)
	{
		bool lSuccess = true;

		for (unsigned int i = 0, lEffectsCount = aJsonEffects.size(); i < lEffectsCount; ++i)
		{
			const Json& lJsonEffect = aJsonEffects[i];
			if (lJsonEffect.count("type") && lJsonEffect.count("description"))
			{
				ActionEffectType aEffectType = getActionEffectTypeFromString(lJsonEffect["type"]);
				if (aEffectType != ActionEffectType::_UNDEFINED)
				{
					ActionEffectInfo lEffectInfo = loadActionEffect(lJsonEffect, aEffectType, aActionIndex, i);
					if (lEffectInfo.isValid())
					{
						aInOutActionEffectInfos.push_back(lEffectInfo);
					}
					else
					{
						OUTPUT_LOG("ERROR: Failed to load ActionEffect at index %i in Action at index %i!", i, aActionIndex);
						lSuccess = false;
					}
				}
				else
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have a valid value for the key 'type'!", i, aActionIndex);
					lSuccess = false;
				}
			}
			else
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", i, aActionIndex);
				lSuccess = false;
			}
		}

		return lSuccess;
	}


	bool loadAction(const Json& aJsonAction, int aActionIndex, std::vector< ActionInfo >& aOutActionInfos)
	{
		bool lSuccess = true;

		ActionInfo lActionInfo;
		lActionInfo.mType = getActionTypeFromString(aJsonAction["type"]);
		if (lActionInfo.mType != ActionType::_UNDEFINED)
		{
			lActionInfo.mDescription = aJsonAction["description"].get< std::string >();
			lActionInfo.mShouldDestroy = aJsonAction["shouldDestroy"].get< bool >();
			lActionInfo.mFirstEntity = aJsonAction["firstEntityId"].get< int >();
			lActionInfo.mSecondEntity = aJsonAction["secondEntityId"].get< int >();

			// Load ActionEffectInfos
			std::vector< ActionEffectInfo > lActionEffectInfos;
			lSuccess = loadActionEffects(aJsonAction["effects"], aActionIndex, lActionEffectInfos);
			if (lSuccess)
			{
				for (const ActionEffectInfo& lInfo : lActionEffectInfos)
				{
					lActionInfo.addActionEffectInfo(lInfo);
				}
				aOutActionInfos.push_back(lActionInfo);
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: Action at index %i doesn't have a valid value for the key 'type'!", aActionIndex);
			lSuccess = false;
		}

		return lSuccess;
	}


	bool loadActions(const Json& aJson, std::vector< ActionInfo >& aOutActionInfos)
	{
		bool lSuccess = true;

		if (aJson.count("actions"))
		{
			const Json& lJsonActions = aJson["actions"];
			for (unsigned int i = 0, lActionsCount = lJsonActions.size(); i < lActionsCount; ++i)
			{
				const Json& lAction = lJsonActions[i];
				if (lAction.count("type") && lAction.count("description") && lAction.count("shouldDestroy") && lAction.count("firstEntityId") && lAction.count("secondEntityId") && lAction.count("effects"))
				{
					lSuccess &= loadAction(lAction, i, aOutActionInfos);
				}
				else
				{
					OUTPUT_LOG("ERROR: Action at index %i doesn't have all the required keys!", i);
				}
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'entityInfos'!");
		}

		return lSuccess;
	}
}//	anonymous


GameData GameData::LoadFromJson(const char* aPath)
{
	bool lSuccess = true;

	GameData lGameData;
	Json lJson = loadJson(aPath);
	if (!lJson.is_null())
	{
		if (loadMessages(lJson, lGameData.mWelcomeMessage, lGameData.mExitMessage))
		{
			if (loadEntities(lJson, lGameData.mPlayerInfo, lGameData.mEntityInfos))
			{
				if (loadActions(lJson, lGameData.mActionInfos))
				{
					OUTPUT_LOG("INFO: Successfully loaded the game configuration data from %s", aPath);
				}
				else
				{
					OUTPUT_LOG("ERROR: Failed to load actions from the game configuration file!");
					lSuccess = false;
				}
			}
			else
			{
				OUTPUT_LOG("ERROR: Failed to load entities from the game configuration file!");
				lSuccess = false;
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: Failed to load messages from the game configuration file!");
			lSuccess = false;
		}
	}
	else
	{
		OUTPUT_LOG("ERROR: Failed to open the json file from %s", aPath);
		lSuccess = false;
	}

	if (!lSuccess)
	{
		lGameData = GameData();
	}

	return lGameData;
}
