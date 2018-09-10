#include "GameDataLoader.h"

#include <assert.h>
#include <fstream>
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


GameDataLoader::GameDataLoader()
{
}


GameDataLoader::~GameDataLoader()
{
}


Player* GameDataLoader::loadGameData(EntityFactory* entityFactory, ActionFactory* actionFactory)
{
	assert(entityFactory && actionFactory);
	Player* player = nullptr;
	const char* path = CONFIG_FILE_PATH;

	Json json = loadJson(path);
	if (!json.is_null())
	{
		if (loadMessages(json))
		{
			player = loadAndCreateEntities(json, entityFactory);
			if (player)
			{
				if (loadAndCreateActions(json, actionFactory))
				{
					hardcodedMethod(entityFactory, actionFactory);
				}
				else
				{
					// Clean Up
					player = nullptr;
					entityFactory->close();
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
			welcomeMessage = (messages["welcomeMessage"]).get<std::string>();
		}
		else
		{
			OutputLog("ERROR: The gameConfig file does not contain the key 'welcomeMessage' within 'messages'!");
			success = false;
		}

		if (success && messages.count("gameEndMessage"))
		{
			gameEndMessage = (messages["gameEndMessage"]).get<std::string>();
		}
		else
		{
			OutputLog("ERROR: The gameConfig file does not contain the key 'gameEndMessage' within 'messages'!");
			success = false;
		}

		if (success && messages.count("exitMessage"))
		{
			exitMessage = (messages["exitMessage"]).get<std::string>();
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


Player* GameDataLoader::loadAndCreateEntities(const Json& json, EntityFactory* entityFactory)
{
	assert(entityFactory);
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
				entityFactory->createEntity(entityInfo);
			}

			if (jsonEntityInfos.count("player"))
			{
				const Json& jsonPlayer = jsonEntityInfos["player"];
				if (jsonPlayer.count("id") && jsonPlayer.count("parentId") && jsonPlayer.count("maxItems") && jsonPlayer.count("startingRoomId"))
				{
					EntityInfo playerInfo = EntityInfo::createPlayerInfo(jsonPlayer["id"], EntityType::PLAYER, jsonPlayer["parentId"], "", "", jsonPlayer["maxItems"], jsonPlayer["startingRoomId"]);
					player = static_cast<Player*>(entityFactory->createEntity(playerInfo));
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
				entityFactory->close();
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

	for (unsigned int i = 0; i < jsonRooms.size(); ++i)
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

	for (unsigned int i = 0; i < jsonExits.size(); ++i)
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

	for (unsigned int i = 0; i < jsonInteractables.size(); ++i)
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

	for (unsigned int i = 0; i < jsonItems.size(); ++i)
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

bool GameDataLoader::loadAndCreateActions(const Json& json, ActionFactory* actionFactory)
{
	assert(actionFactory);
	bool success = true;

	if (json.count("actions"))
	{
		const Json& jsonActions = json["actions"];
		for (unsigned int i = 0; i < jsonActions.size(); ++i)
		{
			const Json& action = jsonActions[i];
			if (action.count("type") && action.count("description") && action.count("shouldDestroy") && action.count("firstEntityId") && action.count("secondEntityId") && action.count("effects"))
			{
				success &= loadAction(action, actionFactory, i);
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
		actionFactory->close();
	}

	return success;
}

bool GameDataLoader::loadAction(const Json& jsonAction, ActionFactory* actionFactory, int actionIndex)
{
	assert(actionFactory);
	bool success = false;

	ActionType type = getActionTypeFromString(jsonAction["type"]);
	if (type != ActionType::_UNDEFINED)
	{
		// Load ActionEffects
		std::vector<ActionEffect*> actionEffects;
		success = loadActionEffects(jsonAction["effects"], actionEffects, actionIndex);
		if (success)
		{
			actionFactory->createAction(type, jsonAction["description"], actionEffects, jsonAction["shouldDestroy"], jsonAction["firstEntityId"], jsonAction["secondEntityId"]);
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

	for (unsigned int i = 0; i < jsonEffects.size(); ++i)
	{
		const Json& effect = jsonEffects[i];
		if (effect.count("type") && effect.count("description"))
		{
			ActionEffectType effectType = getActionEffectTypeFromString("type");
			if (effectType != ActionEffectType::_UNDEFINED)
			{
				// Now we can switch over the action types and load them appropriately
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
	}

	return success;
}


const std::string& GameDataLoader::getWelcomeMessage()
{
	return welcomeMessage;
}


const std::string& GameDataLoader::getGameEndMessage()
{
	return gameEndMessage;
}


const std::string& GameDataLoader::getExitMessage()
{
	return exitMessage;
}

void GameDataLoader::hardcodedMethod(EntityFactory* entityFactory, ActionFactory* actionFactory)
{
	// *** ACTIONS *** //
	std::vector<ActionEffect*> noEffect = std::vector<ActionEffect*>();

	// Storage Room
	{
		EffectReplaceEntity* cabinetReplace = new EffectReplaceEntity("", entityFactory->getEntity(11), entityFactory->getEntity(12));
		EffectAddEntitiesToRoom* keychainAdd = new EffectAddEntitiesToRoom("You see a KEYCHAIN inside the CABINET.", std::vector<Entity*>{entityFactory->getEntity(13)}, static_cast<Room*>(entityFactory->getEntity(1)));
		actionFactory->createAction(ActionType::INTERACTABLE_OPEN, "With a bit of force, you open the CABINET doors.", std::vector<ActionEffect*>{cabinetReplace, keychainAdd}, true, 11);

		EffectRemoveEntities* man3Remove = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(20)});
		actionFactory->createAction(ActionType::GO, "Upon entering the Storage Room, you hear many footsteps outside and the noise of someone being dragged around.", std::vector<ActionEffect*>{man3Remove}, true, 2, 1);

		actionFactory->createAction(ActionType::GO, "You notice the MAN is now gone.", std::vector<ActionEffect*>(), true, 1, 2);
	}

	// Garden
	{
		actionFactory->createAction(ActionType::ITEM_USE, "The MAN sees you swinging the SHOVEL at him and dodges the hit.", std::vector<ActionEffect*>(), false, 23, 18);
		actionFactory->createAction(ActionType::ITEM_USE, "You consider stabbing the MAN with the SCREWDRIVER, but then realize you don't want to murder anyone.", noEffect, false, 24, 18);

		actionFactory->createAction(ActionType::ITEM_USE, "You consider stabbing the defenseless MAN with the SCREWDRIVER but choose not to.", noEffect, false, 24, 19);

		actionFactory->createAction(ActionType::ITEM_USE, "The MAN is unconscious, there's no need to hit him with a SHOVEL.", noEffect, false, 23, 20);
		actionFactory->createAction(ActionType::ITEM_USE, "The MAN is unconscious, there is absolutely no reason to stab him with a SCREWDRIVER.", noEffect, false, 24, 20);

		EffectRemoveEntities* whiskeyRemove = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(45)});
		actionFactory->createAction(ActionType::ITEM_USE, "You give the WHISKEY bottle to the MAN. He just takes it and remains standing in front of the door.", std::vector<ActionEffect*>{whiskeyRemove}, true, 45, 18);

		EffectReplaceEntity* man1Replace = new EffectReplaceEntity("The MAN remains standing but is blinded and trying to clear the FLOUR out of his eyes.", entityFactory->getEntity(18), entityFactory->getEntity(19));
		EffectRemoveEntities* flourRemove = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(31)});
		actionFactory->createAction(ActionType::ITEM_USE, "You throw a handfull of FLOUR towards the MAN's face.", std::vector<ActionEffect*>{man1Replace, flourRemove}, true, 31, 18);

		EffectReplaceEntity* man2Replace = new EffectReplaceEntity("", entityFactory->getEntity(19), entityFactory->getEntity(20));
		EffectUnlockExit* unlockGardenToStorageRoom = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(15)));
		actionFactory->createAction(ActionType::ITEM_USE, "You use the SHOVEL to hit the MAN on the head and he falls to the ground. He is not bleeding and seems to still be breathing. Well played.", std::vector<ActionEffect*>{man2Replace, unlockGardenToStorageRoom}, true, 23, 19);
	}

	// Kitchen
	{
		actionFactory->createAction(ActionType::GO, "You hear a voice coming from the Dining Room saying: 'Go to the exit'", noEffect, true, 7, 4);
		actionFactory->createAction(ActionType::GO, "There's no one around anymore. Who said those words?", noEffect, true, 4, 7);

		actionFactory->createAction(ActionType::INTERACTABLE_OPEN, "You try to open the CUPBOARD, but the doors just won't move. You may need a more drastic approach about those glass doors.", noEffect, false, 27);

		EffectReplaceEntity* cupboardReplace = new EffectReplaceEntity("", entityFactory->getEntity(27), entityFactory->getEntity(28));
		EffectAddEntitiesToRoom* flashlightAndFlourAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(29), entityFactory->getEntity(31)}, static_cast<Room*>(entityFactory->getEntity(4)));
		actionFactory->createAction(ActionType::ITEM_USE, "You swing the TROPHY at the CUPBOARD glass doors and they shatter.", std::vector<ActionEffect*>{cupboardReplace, flashlightAndFlourAdd}, true, 65, 27);

		EffectReplaceEntity* flashlightReplace = new EffectReplaceEntity("The flashlight now emits a fairly bright light that will allow you to see in the dark.", entityFactory->getEntity(29), entityFactory->getEntity(30));
		EffectPlaceItemInItem* batteriesInFlashlightPlace = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(69)), static_cast<Item*>(entityFactory->getEntity(30)));
		EffectLockExit* EmptyRoomToGardenLock = new EffectLockExit("", static_cast<Exit*>(entityFactory->getEntity(37)));
		actionFactory->createAction(ActionType::ITEM_PUT, "You placed the BATTERIES in the FLASHLIGHT.", std::vector<ActionEffect*>{flashlightReplace, batteriesInFlashlightPlace, EmptyRoomToGardenLock}, true, 69, 29);

		EffectReplaceEntity* drawersReplace = new EffectReplaceEntity("", entityFactory->getEntity(32), entityFactory->getEntity(33));
		EffectAddEntitiesToRoom* panAndPotAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(34), entityFactory->getEntity(35)}, static_cast<Room*>(entityFactory->getEntity(4)));
		actionFactory->createAction(ActionType::INTERACTABLE_OPEN, "You open all of the DRAWERS and only find a big PAN and a very small POT.", std::vector<ActionEffect*>{drawersReplace, panAndPotAdd}, true, 32);
	}

	// Empty Room
	{
		EffectLockExit* EmptyRoomToGardenLock = new EffectLockExit("", static_cast<Exit*>(entityFactory->getEntity(37)));
		actionFactory->createAction(ActionType::TAKE, "", std::vector<ActionEffect*>{EmptyRoomToGardenLock}, false, 30);

		EffectUnlockExit* EmptyRoomToGardenUnlock = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(37)));
		actionFactory->createAction(ActionType::DROP, "", std::vector<ActionEffect*>{EmptyRoomToGardenUnlock}, false, 30);

		/*
		EffectRemoveEntities* ratRemove = new EffectRemoveEntities("The RAT runs away through a gap on the door.", std::vector<Entity*>{entityFactory->getEntity(39)});
		EffectAddEntitiesToRoom* panFromInvToRoomAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(34)}, static_cast<Room*>(entityFactory->getEntity(5)));
		EffectUnlockExit* unlockEmptyRoomToGarden = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(37)));
		actionFactory->createAction(ActionType::ITEM_USE, "You throw the PAN at the RAT.", std::vector<ActionEffect*>{ratRemove, panFromInvToRoomAdd, unlockEmptyRoomToGarden}, true, 34, 39);
		*/
	}

	// Office
	{
		EffectReplaceEntity* latchReplace = new EffectReplaceEntity("", entityFactory->getEntity(43), entityFactory->getEntity(44));
		EffectUnlockExit* unlockOfficeToTrophyRoom = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(42)));
		EffectUnlockExit* unlockTrophyRoomToOffice = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(64)));
		actionFactory->createAction(ActionType::INTERACTABLE_OPEN, "You pull on the LATCH to open it.", std::vector<ActionEffect*>{latchReplace, unlockOfficeToTrophyRoom, unlockTrophyRoomToOffice}, true, 43);

		EffectReplaceEntity* drawerReplace = new EffectReplaceEntity("", entityFactory->getEntity(46), entityFactory->getEntity(47));
		EffectAddEntitiesToRoom* keyAAdd = new EffectAddEntitiesToRoom("You can see a small key labeled KEY_A", std::vector<Entity*>{entityFactory->getEntity(48)}, static_cast<Room*>(entityFactory->getEntity(6)));
		actionFactory->createAction(ActionType::ITEM_USE, "You use the SCREWDRIVER to force the DRAWER open.", std::vector<ActionEffect*>{drawerReplace, keyAAdd}, true, 24, 46);

		EffectPlaceItemInItem* keyAInKeychainPlace = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(48)), static_cast<Item*>(entityFactory->getEntity(13)));
		actionFactory->createAction(ActionType::ITEM_PUT, "You placed the KEY_A in the KEYCHAIN", std::vector<ActionEffect*>{keyAInKeychainPlace}, true, 48, 13);

	}

	// Dining Room
	{
		EffectReplaceEntity* dogReplace = new EffectReplaceEntity("The DOG stops being angry and moves aside with the BONE in his mouth.", entityFactory->getEntity(51), entityFactory->getEntity(52));
		EffectRemoveEntities* boneRemove = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(68)});
		EffectUnlockExit* unlockDiningRoomToKitchen = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(50)));
		actionFactory->createAction(ActionType::ITEM_USE, "Slowly, you give the BONE to the big DOG.", std::vector<ActionEffect*>{dogReplace, boneRemove, unlockDiningRoomToKitchen}, true, 68, 51);

		EffectPlaceItemInItem* keyBInKeychainPlace = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(53)), static_cast<Item*>(entityFactory->getEntity(13)));
		actionFactory->createAction(ActionType::ITEM_PUT, "You placed the KEY_B in the KEYCHAIN", std::vector<ActionEffect*>{keyBInKeychainPlace}, true, 53, 13);
	}

	// Main Hall
	{
		EffectReplaceEntity* lock1Replace = new EffectReplaceEntity("", entityFactory->getEntity(59), entityFactory->getEntity(60));
		actionFactory->createAction(ActionType::ITEM_USE, "You use the KEY_A to open the first part of the triple LOCK.", std::vector<ActionEffect*>{lock1Replace}, true, 48, 59);

		EffectReplaceEntity* lock2Replace = new EffectReplaceEntity("", entityFactory->getEntity(60), entityFactory->getEntity(61));
		actionFactory->createAction(ActionType::ITEM_USE, "You use the KEY_B to open the second part of the triple LOCK.", std::vector<ActionEffect*>{lock2Replace}, true, 53, 60);

		EffectGameEnd* gameEnd = new EffectGameEnd(getGameEndMessage());
		actionFactory->createAction(ActionType::ITEM_USE, "You use the KEY_C to open the final part of the triple LOCK.", std::vector<ActionEffect*>{gameEnd}, true, 14, 61);
	}

	// Trophy Room
	{
		EffectReplaceEntity* bagReplace = new EffectReplaceEntity("", entityFactory->getEntity(66), entityFactory->getEntity(67));
		EffectAddEntitiesToRoom* boneAndBatteriesAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(68), entityFactory->getEntity(69)}, static_cast<Room*>(entityFactory->getEntity(9)));
		actionFactory->createAction(ActionType::INTERACTABLE_OPEN, "You undo the knot, open the BAG and find some BATTERIES and a dog toy shaped like a BONE inside.", std::vector<ActionEffect*>{bagReplace, boneAndBatteriesAdd}, true, 66);

		EffectReplaceEntity* panelReplace = new EffectReplaceEntity("", entityFactory->getEntity(70), entityFactory->getEntity(71));
		EffectAddEntitiesToRoom* keyholeAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(72)}, static_cast<Room*>(entityFactory->getEntity(9)));
		actionFactory->createAction(ActionType::INTERACTABLE_OPEN, "You open the PANEL door and see a blue KEYHOLE in the back wall.", std::vector<ActionEffect*>{panelReplace, keyholeAdd}, true, 70);

		EffectUnlockExit* unlockMainHallToEmptyRoom = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(57)));
		actionFactory->createAction(ActionType::ITEM_USE, "You use the BLUE_KEY on the blue KEYHOLE and hear a loud clicking noise coming from elsewhere.", std::vector<ActionEffect*>{unlockMainHallToEmptyRoom}, true, 54, 72);

	}
}
