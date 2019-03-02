#include "GameDataLoader.h"

#include <fstream>
#include "globals.h"
#include "ActionEffect.h"
#include "ActionEffectType.h"
#include "ActionFactory.h"
#include "ActionType.h"
#include "Direction.h"
#include "EffectAddEntitiesToRoom.h"
#include "EffectGameEnd.h"
#include "EffectLockExit.h"
#include "EffectPlaceItemInItem.h"
#include "EffectRemoveEntities.h"
#include "EffectReplaceEntity.h"
#include "EffectUnlockExit.h"
#include "EntityFactory.h"
#include "EntityInfo.h"
#include "EntityType.h"
#include "Exit.h"
#include "Item.h"
#include "Player.h"
#include "Room.h"
#include "json.hpp"


GameDataLoader::GameDataLoader(EntityFactory* aEntityFactory, ActionFactory* aActionFactory)
	:mEntityFactory(aEntityFactory), mActionFactory(aActionFactory)
{
	ASSERT(mEntityFactory && mActionFactory);
}


GameDataLoader::~GameDataLoader()
{
}


Player* GameDataLoader::loadGameData(const char* aPath)
{
	Player* lPlayer = nullptr;

	Json lJson = loadJson(aPath);
	if (!lJson.is_null())
	{
		if (loadMessages(lJson))
		{
			lPlayer = loadAndCreateEntities(lJson);
			if (lPlayer)
			{
				if (loadAndCreateActions(lJson))
				{
					OUTPUT_LOG("INFO: Successfully loaded the game configuration data from %s", aPath);
				}
				else
				{
					// Clean Up
					lPlayer = nullptr;
					mEntityFactory->close();
					OUTPUT_LOG("ERROR: Failed to load actions from the game configuration file!");
				}
			}
			else
			{
				OUTPUT_LOG("ERROR: Failed to load entities from the game configuration file!");
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: Failed to load messages from the game configuration file!");
		}
	}
	else
	{
		OUTPUT_LOG("ERROR: Failed to open the json file from %s", aPath);
	}
	
	return lPlayer;
}


Json GameDataLoader::loadJson(const char* aPath)
{
	Json lJson;

	std::ifstream lFile(aPath);
	if (lFile.good())
	{
		lFile >> lJson;
	}
	return lJson;
}


bool GameDataLoader::loadMessages(const Json& aJson)
{
	bool lSuccess = true;

	if (aJson.count("messages"))
	{
		const Json& messages = aJson["messages"];
		if (lSuccess && messages.count("welcomeMessage"))
		{
			mWelcomeMessage = (messages["welcomeMessage"]).get<std::string>();
		}
		else
		{
			OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'welcomeMessage' within 'messages'!");
			lSuccess = false;
		}

		if (lSuccess && messages.count("exitMessage"))
		{
			mExitMessage = (messages["exitMessage"]).get<std::string>();
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


Player* GameDataLoader::loadAndCreateEntities(const Json& aJson)
{
	Player* lPlayer = nullptr;

	if (aJson.count("entityInfos"))
	{
		std::vector< EntityInfo > lEntityInfos;
		const Json& lJsonEntityInfos = aJson["entityInfos"];
		
		bool lSuccess = loadEntitiesByKey(lJsonEntityInfos, lEntityInfos, "rooms", &GameDataLoader::loadRoomInfos)
			&& loadEntitiesByKey(lJsonEntityInfos, lEntityInfos, "exits", &GameDataLoader::loadExitInfos)
			&& loadEntitiesByKey(lJsonEntityInfos, lEntityInfos, "interactables", &GameDataLoader::loadInteractableInfos)
			&& loadEntitiesByKey(lJsonEntityInfos, lEntityInfos, "items", &GameDataLoader::loadItemInfos);
		
		// Create all entities
		if (lSuccess)
		{
			for (EntityInfo& lEntityInfo : lEntityInfos)
			{
				mEntityFactory->createEntity(lEntityInfo);
			}

			if (lJsonEntityInfos.count("player"))
			{
				const Json& lJsonPlayer = lJsonEntityInfos["player"];
				if (lJsonPlayer.count("id") && lJsonPlayer.count("parentId") && lJsonPlayer.count("maxItems") && lJsonPlayer.count("startingRoomId"))
				{
					EntityInfo lPlayerInfo = EntityInfo::createPlayerInfo(lJsonPlayer["id"], EntityType::PLAYER, lJsonPlayer["parentId"], "", "", lJsonPlayer["maxItems"], lJsonPlayer["startingRoomId"]);
					lPlayer = static_cast<Player*>(mEntityFactory->createEntity(lPlayerInfo));
				}
				else
				{
					OUTPUT_LOG("ERROR: Player doesn't have all the required keys!");
				}
			}
			else
			{
				OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'player' within 'entityInfos'!");
			}

			if (!lPlayer)
			{
				mEntityFactory->close();
			}
		}
	}
	else
	{
		OUTPUT_LOG("ERROR: The gameConfig file does not contain the key 'entityInfos'!");
	}

	return lPlayer;
}

bool GameDataLoader::loadEntitiesByKey(const Json& aJsonEntityInfos, std::vector< EntityInfo >& aInOutEntityInfos, const std::string& aKey, entityLoaderFunc aLoaderFunc)
{
	bool lSuccess = true;

	if (aJsonEntityInfos.count(aKey))
	{
		const Json& lJsonRooms = aJsonEntityInfos[aKey];
		if (!((this->*aLoaderFunc)(lJsonRooms, aInOutEntityInfos)))
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


bool GameDataLoader::loadRoomInfos(const Json& aJsonRooms, std::vector< EntityInfo >& aInOutEntityInfos)
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


bool GameDataLoader::loadExitInfos(const Json& aJsonExits, std::vector< EntityInfo >& aInOutEntityInfos)
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


bool GameDataLoader::loadInteractableInfos(const Json& aJsonInteractables, std::vector< EntityInfo >& aInOutEntityInfos)
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


bool GameDataLoader::loadItemInfos(const Json& aJsonItems, std::vector< EntityInfo >& aInOutEntityInfos)
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

bool GameDataLoader::loadAndCreateActions(const Json& aJson)
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
				lSuccess &= loadAction(lAction, i);
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

	if (!lSuccess)
	{
		mActionFactory->close();
	}

	return lSuccess;
}

bool GameDataLoader::loadAction(const Json& aJsonAction, int aActionIndex)
{
	bool lSuccess = false;

	ActionType lActionType = getActionTypeFromString(aJsonAction["type"]);
	if (lActionType != ActionType::_UNDEFINED)
	{
		// Load ActionEffects
		std::vector< ActionEffect* > lActionEffects;
		lSuccess = loadActionEffects(aJsonAction["effects"], lActionEffects, aActionIndex);
		if (lSuccess)
		{
			mActionFactory->createAction(lActionType, aJsonAction["description"], lActionEffects, aJsonAction["shouldDestroy"], aJsonAction["firstEntityId"], aJsonAction["secondEntityId"]);
		}
	}
	else
	{
		OUTPUT_LOG("ERROR: Action at index %i doesn't have a valid value for the key 'type'!", aActionIndex);
	}

	return lSuccess;
}


bool GameDataLoader::loadActionEffects(const Json& aJsonEffects, std::vector< ActionEffect* >& aInOutActionEffects, int aActionIndex)
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
				ActionEffect* lActionEffect = loadActionEffect(lJsonEffect, aEffectType, aActionIndex, i);
				if (lActionEffect)
				{
					aInOutActionEffects.push_back(lActionEffect);
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

	// Clean Up after failure
	if (!lSuccess)
	{
		for each (ActionEffect* lActionEffect in aInOutActionEffects)
		{
			delete lActionEffect;
		}
		aInOutActionEffects.clear();
	}

	return lSuccess;
}


ActionEffect* GameDataLoader::loadActionEffect(const Json& aJsonEffect, ActionEffectType aEffectType, int aActionIndex, int aEffectIndex)
{
	ASSERT(aEffectType != ActionEffectType::_UNDEFINED);
	ActionEffect* lActionEffect = nullptr;

	// Now we can switch over the lAction types and load them appropriately
	switch (aEffectType)
	{

	case ActionEffectType::ADD_ENTITIES:
	{
		if (aJsonEffect.count("entitiesToAddIds") && aJsonEffect.count("targetRoomId"))
		{
			bool lSuccess = true;

			const Json& lEntitiesToAddIds = aJsonEffect["entitiesToAddIds"];
			std::vector< Entity* > lEntitiesToAdd;
			for (int i = 0, lEntitiesCount = lEntitiesToAddIds.size(); i < lEntitiesCount; ++i)
			{
				Entity* lEntity = mEntityFactory->getEntity(lEntitiesToAddIds[i]);
				if (lEntity)
				{
					lEntitiesToAdd.push_back(lEntity);
				}
				else
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entitiesToAdd' at index %i that does't point to any existing Entity!", aEffectIndex, aActionIndex, i);
					lSuccess = false;
				}
			}

			Room* lTargetRoom = static_cast<Room*>(mEntityFactory->getEntity(aJsonEffect["targetRoomId"]));
			if (!lTargetRoom)
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'targetRoomId' that does't point to any existing Room!", aEffectIndex, aActionIndex);
				lSuccess = false;
			}

			if (lSuccess)
			{
				// We continue
				lActionEffect = new EffectAddEntitiesToRoom(aJsonEffect["description"], lEntitiesToAdd, lTargetRoom);
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}
		break;
	}

	case ActionEffectType::GAME_END:
	{
		std::string lDescription = aJsonEffect["description"];
		lActionEffect = new EffectGameEnd(lDescription);
		break;
	}

	case ActionEffectType::LOCK_EXIT:
	{
		if (aJsonEffect.count("exitToLockId"))
		{
			Exit* lExitToLock = static_cast<Exit*>(mEntityFactory->getEntity(aJsonEffect["exitToLockId"]));
			if (lExitToLock)
			{
				lActionEffect = new EffectLockExit(aJsonEffect["description"], lExitToLock);
			}
			else
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'exitToLockId' that does't point to any existing Exit!", aEffectIndex, aActionIndex);
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}
		break;
	}

	case ActionEffectType::PLACE_ITEM_IN_ITEM:
	{
		if (aJsonEffect.count("itemId") && aJsonEffect.count("containerId"))
		{
			Item* lItem = static_cast<Item*>(mEntityFactory->getEntity(aJsonEffect["itemId"]));
			Item* lContainer = static_cast<Item*>(mEntityFactory->getEntity(aJsonEffect["containerId"]));
			if (lItem && lContainer)
			{
				lActionEffect = new EffectPlaceItemInItem(aJsonEffect["description"], lItem, lContainer);
			}
			else
			{
				if (!lItem)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'itemId' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
				if (!lContainer)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'containerId' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}
		break;
	}

	case ActionEffectType::REMOVE_ENTITIES:
	{
		if (aJsonEffect.count("entitiesToRemoveIds"))
		{
			bool lSuccess = true;

			const Json& lEntitiesToRemoveIds = aJsonEffect["entitiesToRemoveIds"];
			std::vector< Entity* > lEntitiesToRemove;
			for (unsigned int i = 0, lEntitiesCount = lEntitiesToRemoveIds.size(); i < lEntitiesCount; ++i)
			{
				Entity* lEntity = mEntityFactory->getEntity(lEntitiesToRemoveIds[i]);
				if (lEntity)
				{
					lEntitiesToRemove.push_back(lEntity);
				}
				else
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entitiesToAdd' at index %i that does't point to any existing Entity!", aEffectIndex, aActionIndex, i);
					lSuccess = false;
				}
			}

			if (lSuccess)
			{
				// We continue
				lActionEffect = new EffectRemoveEntities(aJsonEffect["description"], lEntitiesToRemove);
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}
		break;
	}

	case ActionEffectType::REPLACE_ENTITY:
	{
		if (aJsonEffect.count("entityToRemoveId") && aJsonEffect.count("entityToAddId"))
		{
			Entity* lEntityToRemove = mEntityFactory->getEntity(aJsonEffect["entityToRemoveId"]);
			Entity* lEntityToAdd = mEntityFactory->getEntity(aJsonEffect["entityToAddId"]);
			if (lEntityToRemove && lEntityToAdd)
			{
				lActionEffect = new EffectReplaceEntity(aJsonEffect["description"], lEntityToRemove, lEntityToAdd);
			}
			else
			{
				if (!lEntityToRemove)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entityToRemoveId' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
				if (!lEntityToAdd)
				{
					OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entityToAdd' that doesn't point to any existing Entity!", aEffectIndex, aActionIndex);
				}
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}
		break;
	}

	case ActionEffectType::UNLOCK_EXIT:
	{
		if (aJsonEffect.count("exitToUnlockId"))
		{
			Exit* lExitToUnlock = static_cast<Exit*>(mEntityFactory->getEntity(aJsonEffect["exitToUnlockId"]));
			if (lExitToUnlock)
			{
				lActionEffect = new EffectUnlockExit(aJsonEffect["description"], lExitToUnlock);
			}
			else
			{
				OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'exitToUnlockId' that does't point to any existing Exit!", aEffectIndex, aActionIndex);
			}
		}
		else
		{
			OUTPUT_LOG("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", aEffectIndex, aActionIndex);
		}
		break;
	}

	}

	return lActionEffect;
}


const std::string& GameDataLoader::getWelcomeMessage() const
{
	return mWelcomeMessage;
}


const std::string& GameDataLoader::getExitMessage() const
{
	return mExitMessage;
}
