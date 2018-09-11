#include "GameDataLoader.h"

#include <assert.h>
#include <fstream>
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
#include "globals.h"
#include "json.hpp"


GameDataLoader::GameDataLoader(EntityFactory* entityFactory, ActionFactory* actionFactory)
	:m_entityFactory(entityFactory), m_actionFactory(actionFactory)
{
	assert(m_entityFactory && m_actionFactory);
}


GameDataLoader::~GameDataLoader()
{
}


Player* GameDataLoader::loadGameData(const char* path)
{
	Player* player = nullptr;

	Json json = loadJson(path);
	if (!json.is_null())
	{
		if (loadMessages(json))
		{
			player = loadAndCreateEntities(json);
			if (player)
			{
				if (loadAndCreateActions(json))
				{
					OutputLog("INFO: Successfully loaded the game configuration data from %s", path);
				}
				else
				{
					// Clean Up
					player = nullptr;
					m_entityFactory->close();
					OutputLog("ERROR: Failed to load actions from the game configuration file!");
				}
			}
			else
			{
				OutputLog("ERROR: Failed to load entities from the game configuration file!");
			}
		}
		else
		{
			OutputLog("ERROR: Failed to load messages from the game configuration file!");
		}
	}
	else
	{
		OutputLog("ERROR: Failed to open the json file from %s", path);
	}
	
	return player;
}


Json GameDataLoader::loadJson(const char* path)
{
	Json json;

	std::ifstream file(path);
	if (file.good())
	{
		assert(file.good());
		file >> json;
	}
	return json;
}


bool GameDataLoader::loadMessages(const Json& json)
{
	bool success = true;

	if (json.count("messages"))
	{
		const Json& messages = json["messages"];
		if (success && messages.count("welcomeMessage"))
		{
			m_welcomeMessage = (messages["welcomeMessage"]).get<std::string>();
		}
		else
		{
			OutputLog("ERROR: The gameConfig file does not contain the key 'welcomeMessage' within 'messages'!");
			success = false;
		}

		if (success && messages.count("exitMessage"))
		{
			m_exitMessage = (messages["exitMessage"]).get<std::string>();
		}
		else
		{
			OutputLog("ERROR: The gameConfig file does not contain the key 'exitMessage' within 'messages'!");
			success = false;
		}
	}
	else
	{
		OutputLog("ERROR: The gameConfig file does not contain the key 'messages'!");
		success = false;
	}
	return success;
}


Player* GameDataLoader::loadAndCreateEntities(const Json& json)
{
	Player* player = nullptr;

	if (json.count("entityInfos"))
	{
		std::vector<EntityInfo> entityInfos;
		const Json& jsonEntityInfos = json["entityInfos"];
		
		bool success = loadEntitiesByKey(jsonEntityInfos, entityInfos, "rooms", &GameDataLoader::loadRoomInfos)
			&& loadEntitiesByKey(jsonEntityInfos, entityInfos, "exits", &GameDataLoader::loadExitInfos)
			&& loadEntitiesByKey(jsonEntityInfos, entityInfos, "interactables", &GameDataLoader::loadInteractableInfos)
			&& loadEntitiesByKey(jsonEntityInfos, entityInfos, "items", &GameDataLoader::loadItemInfos);
		
		// Create all entities
		if (success)
		{
			for (EntityInfo& entityInfo : entityInfos)
			{
				m_entityFactory->createEntity(entityInfo);
			}

			if (jsonEntityInfos.count("player"))
			{
				const Json& jsonPlayer = jsonEntityInfos["player"];
				if (jsonPlayer.count("id") && jsonPlayer.count("parentId") && jsonPlayer.count("maxItems") && jsonPlayer.count("startingRoomId"))
				{
					EntityInfo playerInfo = EntityInfo::createPlayerInfo(jsonPlayer["id"], EntityType::PLAYER, jsonPlayer["parentId"], "", "", jsonPlayer["maxItems"], jsonPlayer["startingRoomId"]);
					player = static_cast<Player*>(m_entityFactory->createEntity(playerInfo));
				}
				else
				{
					OutputLog("ERROR: Player doesn't have all the required keys!");
				}
			}
			else
			{
				OutputLog("ERROR: The gameConfig file does not contain the key 'player' within 'entityInfos'!");
			}

			if (!player)
			{
				m_entityFactory->close();
			}
		}
	}
	else
	{
		OutputLog("ERROR: The gameConfig file does not contain the key 'entityInfos'!");
	}

	return player;
}

bool GameDataLoader::loadEntitiesByKey(const Json& jsonEntityInfos, std::vector<EntityInfo>& entityInfos, const std::string& key, entityLoaderFunc loaderFunc)
{
	bool success = true;

	if (jsonEntityInfos.count(key))
	{
		const Json& jsonRooms = jsonEntityInfos[key];
		if (!((this->*loaderFunc)(jsonRooms, entityInfos)))
		{
			OutputLog("ERROR: Failed to load rooms from the game configuration file!");
			success = false;
		}
	}
	else
	{
		OutputLog("ERROR: The gameConfig file does not contain the key '%s' within 'entityInfos'!", key.c_str());
		success = false;
	}

	return success;
}


bool GameDataLoader::loadRoomInfos(const Json& jsonRooms, std::vector<EntityInfo>& entityInfos)
{
	bool success = true;

	unsigned int roomsCount = jsonRooms.size();
	for (unsigned int i = 0; i < roomsCount; ++i)
	{
		const Json& room = jsonRooms[i];
		if (room.count("id") && room.count("name") && room.count("description") && room.count("isDark"))
		{
			EntityInfo ei = EntityInfo::createRoomInfo(room["id"], EntityType::ROOM, -1, room["name"], room["description"], room["isDark"]);
			entityInfos.push_back(ei);
		}
		else
		{
			OutputLog("ERROR: Room at index %i doesn't have all the required keys!", i);
			success = false;
		}
	}

	return success;
}


bool GameDataLoader::loadExitInfos(const Json& jsonExits, std::vector<EntityInfo>& entityInfos)
{
	bool success = true;

	unsigned int exitsCount = jsonExits.size();
	for (unsigned int i = 0; i < exitsCount; ++i)
	{
		const Json& exit = jsonExits[i];
		if (exit.count("id") && exit.count("parentId") && exit.count("name") && exit.count("description") && exit.count("direction") && exit.count("isLocked") && exit.count("lockedDescription") && exit.count("targetRoomId"))
		{
			if (getDirectionFromString(exit["direction"]) != Direction::_UNDEFINED)
			{
				EntityInfo ei = EntityInfo::createExitInfo(exit["id"], EntityType::EXIT, exit["parentId"], exit["name"], exit["description"], getDirectionFromString(exit["direction"]), exit["isLocked"], exit["lockedDescription"], exit["targetRoomId"]);
				entityInfos.push_back(ei);
			}
			else
			{
				OutputLog("ERROR: Exit at index %i doesn't have a valid value for the key 'direction'!", i);
			}
		}
		else
		{
			OutputLog("ERROR: Exit at index %i doesn't have all the required keys!", i);
			success = false;
		}
	}

	return success;
}


bool GameDataLoader::loadInteractableInfos(const Json& jsonInteractables, std::vector<EntityInfo>& entityInfos)
{
	bool success = true;

	unsigned int interactablesCount = jsonInteractables.size();
	for (unsigned int i = 0; i < interactablesCount; ++i)
	{
		const Json& interactable = jsonInteractables[i];
		if (interactable.count("id") && interactable.count("parentId") && interactable.count("name") && interactable.count("description") && interactable.count("inspectDescription") && interactable.count("isVisibleInDark"))
		{
			EntityInfo ei = EntityInfo::createInteractableInfo(interactable["id"], EntityType::INTERACTABLE, interactable["parentId"], interactable["name"], interactable["description"], interactable["inspectDescription"], interactable["isVisibleInDark"]);
			entityInfos.push_back(ei);
		}
		else
		{
			OutputLog("ERROR: Interactable at index %i doesn't have all the required keys!", i);
			success = false;
		}
	}

	return success;
}


bool GameDataLoader::loadItemInfos(const Json& jsonItems, std::vector<EntityInfo>& entityInfos)
{
	bool success = true;

	unsigned int itemsCount = jsonItems.size();
	for (unsigned int i = 0; i < itemsCount; ++i)
	{
		const Json& item = jsonItems[i];
		if (item.count("id") && item.count("parentId") && item.count("name") && item.count("description") && item.count("inspectDescription") && item.count("isVisibleInDark") && item.count("hasLight"))
		{
			EntityInfo ei = EntityInfo::createItemInfo(item["id"], EntityType::ITEM, item["parentId"], item["name"], item["description"], item["inspectDescription"], item["isVisibleInDark"], item["hasLight"]);
			entityInfos.push_back(ei);
		}
		else
		{
			OutputLog("ERROR: Item at index %i doesn't have all the required keys!", i);
			success = false;
		}
	}

	return success;
}

bool GameDataLoader::loadAndCreateActions(const Json& json)
{
	bool success = true;

	if (json.count("actions"))
	{
		const Json& jsonActions = json["actions"];
		unsigned int actionsCount = jsonActions.size();
		for (unsigned int i = 0; i < actionsCount; ++i)
		{
			const Json& action = jsonActions[i];
			if (action.count("type") && action.count("description") && action.count("shouldDestroy") && action.count("firstEntityId") && action.count("secondEntityId") && action.count("effects"))
			{
				success &= loadAction(action, i);
			}
			else
			{
				OutputLog("ERROR: Action at index %i doesn't have all the required keys!", i);
			}
		}
	}
	else
	{
		OutputLog("ERROR: The gameConfig file does not contain the key 'entityInfos'!");
	}

	if (!success)
	{
		m_actionFactory->close();
	}

	return success;
}

bool GameDataLoader::loadAction(const Json& jsonAction, int actionIndex)
{
	bool success = false;

	ActionType type = getActionTypeFromString(jsonAction["type"]);
	if (type != ActionType::_UNDEFINED)
	{
		// Load ActionEffects
		std::vector<ActionEffect*> actionEffects;
		success = loadActionEffects(jsonAction["effects"], actionEffects, actionIndex);
		if (success)
		{
			m_actionFactory->createAction(type, jsonAction["description"], actionEffects, jsonAction["shouldDestroy"], jsonAction["firstEntityId"], jsonAction["secondEntityId"]);
		}
	}
	else
	{
		OutputLog("ERROR: Action at index %i doesn't have a valid value for the key 'type'!", actionIndex);
	}

	return success;
}


bool GameDataLoader::loadActionEffects(const Json& jsonEffects, std::vector<ActionEffect*>& actionEffects, int actionIndex)
{
	bool success = true;

	unsigned int effectsCount = jsonEffects.size();
	for (unsigned int i = 0; i < effectsCount; ++i)
	{
		const Json& jsonEffect = jsonEffects[i];
		if (jsonEffect.count("type") && jsonEffect.count("description"))
		{
			ActionEffectType effectType = getActionEffectTypeFromString(jsonEffect["type"]);
			if (effectType != ActionEffectType::_UNDEFINED)
			{
				ActionEffect* actionEffect = loadActionEffect(jsonEffect, effectType, actionIndex, i);
				if (actionEffect)
				{
					actionEffects.push_back(actionEffect);
				}
				else
				{
					OutputLog("ERROR: Failed to load ActionEffect at index %i in Action at index %i!", i, actionIndex);
					success = false;
				}

			}
			else
			{
				OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have a valid value for the key 'type'!", i, actionIndex);
				success = false;
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", i, actionIndex);
			success = false;
		}
	}

	// Clean Up after failure
	if (!success)
	{
		for each (ActionEffect* actionEffect in actionEffects)
		{
			delete actionEffect;
		}
		actionEffects.clear();
	}

	return success;
}


ActionEffect* GameDataLoader::loadActionEffect(const Json& jsonEffect, ActionEffectType effectType, int actionIndex, int effectIndex)
{
	assert(effectType != ActionEffectType::_UNDEFINED);
	ActionEffect* actionEffect = nullptr;

	// Now we can switch over the action types and load them appropriately
	switch (effectType)
	{

	case ActionEffectType::ADD_ENTITIES:
	{
		if (jsonEffect.count("entitiesToAddIds") && jsonEffect.count("targetRoomId"))
		{
			bool success = true;

			const Json& entitiesToAddIds = jsonEffect["entitiesToAddIds"];
			std::vector<Entity*> entitiesToAdd;
			int entitiesCount = entitiesToAddIds.size();
			for (int i = 0; i < entitiesCount; ++i)
			{
				Entity* entity = m_entityFactory->getEntity(entitiesToAddIds[i]);
				if (entity)
				{
					entitiesToAdd.push_back(entity);
				}
				else
				{
					OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entitiesToAdd' at index %i that does't point to any existing Entity!", effectIndex, actionIndex, i);
					success = false;
				}
			}

			Room* targetRoom = static_cast<Room*>(m_entityFactory->getEntity(jsonEffect["targetRoomId"]));
			if (!targetRoom)
			{
				OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'targetRoomId' that does't point to any existing Room!", effectIndex, actionIndex);
				success = false;
			}

			if (success)
			{
				// We continue
				actionEffect = new EffectAddEntitiesToRoom(jsonEffect["description"], entitiesToAdd, targetRoom);
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", effectIndex, actionIndex);
		}
		break;
	}

	case ActionEffectType::GAME_END:
	{
		std::string description = jsonEffect["description"];
		actionEffect = new EffectGameEnd(description);
		break;
	}

	case ActionEffectType::LOCK_EXIT:
	{
		if (jsonEffect.count("exitToLockId"))
		{
			Exit* exitToLock = static_cast<Exit*>(m_entityFactory->getEntity(jsonEffect["exitToLockId"]));
			if (exitToLock)
			{
				actionEffect = new EffectLockExit(jsonEffect["description"], exitToLock);
			}
			else
			{
				OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'exitToLockId' that does't point to any existing Exit!", effectIndex, actionIndex);
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", effectIndex, actionIndex);
		}
		break;
	}

	case ActionEffectType::PLACE_ITEM_IN_ITEM:
	{
		if (jsonEffect.count("itemId") && jsonEffect.count("containerId"))
		{
			Item* item = static_cast<Item*>(m_entityFactory->getEntity(jsonEffect["itemId"]));
			Item* container = static_cast<Item*>(m_entityFactory->getEntity(jsonEffect["containerId"]));
			if (item && container)
			{
				actionEffect = new EffectPlaceItemInItem(jsonEffect["description"], item, container);
			}
			else
			{
				if (!item)
				{
					OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'itemId' that doesn't point to any existing Entity!", effectIndex, actionIndex);
				}
				if (!container)
				{
					OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'containerId' that doesn't point to any existing Entity!", effectIndex, actionIndex);
				}
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", effectIndex, actionIndex);
		}
		break;
	}

	case ActionEffectType::REMOVE_ENTITIES:
	{
		if (jsonEffect.count("entitiesToRemoveIds"))
		{
			bool success = true;

			const Json& entitiesToRemoveIds = jsonEffect["entitiesToRemoveIds"];
			std::vector<Entity*> entitiesToRemove;
			int entitiesCount = entitiesToRemoveIds.size();
			for (int i = 0; i < entitiesCount; ++i)
			{
				Entity* entity = m_entityFactory->getEntity(entitiesToRemoveIds[i]);
				if (entity)
				{
					entitiesToRemove.push_back(entity);
				}
				else
				{
					OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entitiesToAdd' at index %i that does't point to any existing Entity!", effectIndex, actionIndex, i);
					success = false;
				}
			}

			if (success)
			{
				// We continue
				actionEffect = new EffectRemoveEntities(jsonEffect["description"], entitiesToRemove);
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", effectIndex, actionIndex);
		}
		break;
	}

	case ActionEffectType::REPLACE_ENTITY:
	{
		if (jsonEffect.count("entityToRemoveId") && jsonEffect.count("entityToAddId"))
		{
			Entity* entityToRemove = m_entityFactory->getEntity(jsonEffect["entityToRemoveId"]);
			Entity* entityToAdd = m_entityFactory->getEntity(jsonEffect["entityToAddId"]);
			if (entityToRemove && entityToAdd)
			{
				actionEffect = new EffectReplaceEntity(jsonEffect["description"], entityToRemove, entityToAdd);
			}
			else
			{
				if (!entityToRemove)
				{
					OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entityToRemoveId' that doesn't point to any existing Entity!", effectIndex, actionIndex);
				}
				if (!entityToAdd)
				{
					OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'entityToAdd' that doesn't point to any existing Entity!", effectIndex, actionIndex);
				}
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", effectIndex, actionIndex);
		}
		break;
	}

	case ActionEffectType::UNLOCK_EXIT:
	{
		if (jsonEffect.count("exitToUnlockId"))
		{
			Exit* exitToUnlock = static_cast<Exit*>(m_entityFactory->getEntity(jsonEffect["exitToUnlockId"]));
			if (exitToUnlock)
			{
				actionEffect = new EffectUnlockExit(jsonEffect["description"], exitToUnlock);
			}
			else
			{
				OutputLog("ERROR: ActionEffect at index %i in Action at index %i has a value for key 'exitToUnlockId' that does't point to any existing Exit!", effectIndex, actionIndex);
			}
		}
		else
		{
			OutputLog("ERROR: ActionEffect at index %i in Action at index %i doesn't have all the required keys!", effectIndex, actionIndex);
		}
		break;
	}

	}

	return actionEffect;
}


const std::string& GameDataLoader::getWelcomeMessage() const
{
	return m_welcomeMessage;
}


const std::string& GameDataLoader::getExitMessage() const
{
	return m_exitMessage;
}
