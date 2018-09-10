#include "GameDataLoader.h"

#include <assert.h>
#include <fstream>
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

	Json* json = loadJson(path);
	if (json != nullptr)
	{
		if (loadMessages(json))
		{
			if (loadAndCreateEntities(json, entityFactory))
			{
				player = hardcodedMethod(entityFactory, actionFactory);
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


Json* GameDataLoader::loadJson(const char* path)
{
	Json* json = nullptr;

	std::ifstream file(path);
	if (file.good())
	{
		assert(file.good());
		json = new Json();
		file >> (*json);
	}
	return json;
}


bool GameDataLoader::loadMessages(Json* json)
{
	assert(json);
	bool success = true;

	if (json->count("messages"))
	{
		Json& messages = (*json)["messages"];
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


bool GameDataLoader::loadAndCreateEntities(Json* json, EntityFactory* entityFactory)
{
	assert(json && entityFactory);
	bool success = true;

	if (json->count("entityInfos"))
	{
		std::vector<EntityInfo> entityInfos;
		Json& jsonEntityInfos = (*json)["entityInfos"];
		
		success = loadEntitiesByKey(jsonEntityInfos, entityInfos, "rooms", &GameDataLoader::loadRoomInfos)
			&& loadEntitiesByKey(jsonEntityInfos, entityInfos, "exits", &GameDataLoader::loadExitInfos);
		
		// TO DO: Load interactables and items

		// Create all entities
		if (success)
		{
			for (EntityInfo& entityInfo : entityInfos)
			{
				entityFactory->createEntity(entityInfo);
			}
		}
	}
	else
	{
		OutputLog("ERROR: The gameConfig file does not contain the key 'entityInfos'!");
		success = false;
	}

	return success;
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
		if (exit.count("id") && exit.count("parentId") && exit.count("name") && exit.count("description") && exit.count("direction") && getDirectionFromString(exit["direction"]) != Direction::_UNDEFINED && exit.count("isLocked") && exit.count("lockedDescription") && exit.count("targetRoomId"))
		{
			EntityInfo ei = EntityInfo::createExitInfo(exit["id"], EntityType::EXIT, exit["parentId"], exit["name"], exit["description"], getDirectionFromString(exit["direction"]), exit["isLocked"], exit["lockedDescription"], exit["targetRoomId"]);
			entityInfos.push_back(ei);
		}
		else
		{
			OutputLog("ERROR: Exit at index %i doesn't have all the required keys!", i);
			success = false;
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

Player* GameDataLoader::hardcodedMethod(EntityFactory* entityFactory, ActionFactory* actionFactory)
{
	// *** ENTITIES *** //

	std::vector<EntityInfo> worldInfo{
		// Rooms
		//EntityInfo::createRoomInfo(1, EntityType::ROOM, -1, "Storage Room", "A small storage room.", true),
		//EntityInfo::createRoomInfo(2, EntityType::ROOM, -1, "Garden", "A fairly small garden with beautiful roses.", false),
		//EntityInfo::createRoomInfo(3, EntityType::ROOM, -1, "Tool Shed", "A dirty shed.", false),
		//EntityInfo::createRoomInfo(4, EntityType::ROOM, -1, "Kitchen", "A very clean, but fairly empty kitchen.", false),
		//EntityInfo::createRoomInfo(5, EntityType::ROOM, -1, "Empty Room", "An empty room with nothing special in it.", false),
		//EntityInfo::createRoomInfo(6, EntityType::ROOM, -1, "Office", "A nice office with fancy looking wooden walls.", false),
		//EntityInfo::createRoomInfo(7, EntityType::ROOM, -1, "Dining Room", "A room with a long dining table in the middle.", true),
		//EntityInfo::createRoomInfo(8, EntityType::ROOM, -1, "Main Hall", "A big hall without much to see in it.", true),
		//EntityInfo::createRoomInfo(9, EntityType::ROOM, -1, "Trophy Room", "A room that contains prizes and trophies on the walls.", false),
		// Storage Room
		//EntityInfo::createExitInfo(10, EntityType::EXIT, 1, "Storage Room TO Garden", "A rusted metal door", Direction::E, false, "", 2),
		EntityInfo::createInteractableInfo(11, EntityType::INTERACTABLE, 1, "CABINET", "A metal CABINET hangs from the wall", "The CABINET doors are closed.", false),
		EntityInfo::createInteractableInfo(12, EntityType::INTERACTABLE, -1, "CABINET", "A metal CABINET hangs from the wall", "The CABINET doors are opened.", false),
		EntityInfo::createItemInfo(13, EntityType::ITEM, -1, "KEYCHAIN", "A shiny KEYCHAIN", "", false, false),
		EntityInfo::createItemInfo(14, EntityType::ITEM, 13, "KEY_C", "A small key labeled KEY_C", "", false, false),
		// Garden
		//EntityInfo::createExitInfo(15, EntityType::EXIT, 2, "Garden TO Storage Room", "A rusted metal door", Direction::W, true, "The MAN standing in front of the door pushes you away.", 1),
		//EntityInfo::createExitInfo(16, EntityType::EXIT, 2, "Garden TO Tool Shed", "A wooden door covered in moss", Direction::E, false, "", 3),
		//EntityInfo::createExitInfo(17, EntityType::EXIT, 2, "Garden TO Empty Room", "A wooden door with small glass panels on the top half", Direction::S, false, "", 5),
		EntityInfo::createInteractableInfo(18, EntityType::INTERACTABLE, 2, "MAN", "An old MAN stands in from of the western exit", "The MAN doesn't seem to be going anywhere.", false),
		EntityInfo::createInteractableInfo(19, EntityType::INTERACTABLE, -1, "MAN", "The old MAN is still standing in front of the door", "The MAN is trying to clear the flour from his eyes.", false),
		EntityInfo::createInteractableInfo(20, EntityType::INTERACTABLE, -1, "MAN", "An old MAN lying on the ground", "The MAN seems to be unconscious.", false),
		// Tool Shed
		//EntityInfo::createExitInfo(21, EntityType::EXIT, 3, "Tool Shed TO Garden", "A wooden door covered in moss", Direction::W, false, "", 2),
		//EntityInfo::createExitInfo(22, EntityType::EXIT, 3, "Tool Shed TO Office", "A fancy looking door", Direction::S, false, "", 6),
		EntityInfo::createItemInfo(23, EntityType::ITEM, 3, "SHOVEL", "A small, worn out SHOVEL", "", false, false),
		EntityInfo::createItemInfo(24, EntityType::ITEM, 3, "SCREWDRIVER", "A standard Phillips SCREWDRIVER", "", false, false),
		// Kitchen
		//EntityInfo::createExitInfo(25, EntityType::EXIT, 4, "Kitchen TO Empty Room", "A heavy looking wooden door", Direction::E, true, "The door is bolted shut.", 5),
		//EntityInfo::createExitInfo(26, EntityType::EXIT, 4, "Kitchen TO Dining Room", "A swing door", Direction::S, false, "", 7),
		EntityInfo::createInteractableInfo(27, EntityType::INTERACTABLE, 4, "CUPBOARD", "A large CUPBOARD with glass doors", "The CUPBOARD doors are closed shut but you can see a FLASHLIGHT and a bag of FLOUR inside.", false),
		EntityInfo::createInteractableInfo(28, EntityType::INTERACTABLE, -1, "CUPBOARD", "A large CUPBOARD with broken glass doors", "The CUPBOARD doors are still closed but the glass panes are broken.", false),
		EntityInfo::createItemInfo(29, EntityType::ITEM, -1, "FLASHLIGHT", "A small led FLASHLIGHT without batteries", "The FLASHLIGHT looks brand new but is not working.", true, false),
		EntityInfo::createItemInfo(30, EntityType::ITEM, -1, "FLASHLIGHT", "A small and powerful led FLASHLIGHT", "The FLASHLIGHT looks brand new.", true, true),
		EntityInfo::createItemInfo(31, EntityType::ITEM, -1, "FLOUR", "A bag of FLOUR", "", false, false),
		EntityInfo::createInteractableInfo(32, EntityType::INTERACTABLE, 4, "DRAWERS", "A group of DRAWERS next to the sink.", "The DRAWERS are closed.", false),
		EntityInfo::createInteractableInfo(33, EntityType::INTERACTABLE, -1, "DRAWERS", "A group of DRAWERS next to the sink", "The DRAWERS are opened.", false),
		EntityInfo::createItemInfo(34, EntityType::ITEM, -1, "PAN", "A big PAN", "", false, false),
		EntityInfo::createItemInfo(35, EntityType::ITEM, -1, "POT", "A very small POT", "", false, false),
		// Empty Room
		//EntityInfo::createExitInfo(36, EntityType::EXIT, 5, "Empty Room TO Kitchen", "A heavy looking wooden door", Direction::W, true, "The door is bolted shut.", 4),
		//EntityInfo::createExitInfo(37, EntityType::EXIT, 5, "Empty Room TO Garden", "A wooden door with small glass panels on the top half", Direction::N, false, "When you approach the door a big RAT is lured by the light from your flashlight and tries to bite you, so you step back.", 2),
		//EntityInfo::createExitInfo(38, EntityType::EXIT, 5, "Empty Room TO Main Hall", "A thick wooden door", Direction::S, false, "", 8),
		EntityInfo::createInteractableInfo(39, EntityType::INTERACTABLE, 5, "RAT", "A big RAT rests next to the northern door.", "The RAT doesn't look very friendly.", false),
		EntityInfo::createItemInfo(40, EntityType::ITEM, 5, "BALL", "A small stress BALL", "", false, false),
		// Office
		//EntityInfo::createExitInfo(41, EntityType::EXIT, 6, "Office TO Tool shed", "A fancy looking door", Direction::N, false, "", 3),
		//EntityInfo::createExitInfo(42, EntityType::EXIT, 6, "Office TO Trophy Room", "A red-painted door", Direction::S, true, "The door is locked by a LATCH", 9),
		EntityInfo::createInteractableInfo(43, EntityType::INTERACTABLE, 6, "LATCH", "A LATCH keeping the southern door locked", "", false),
		EntityInfo::createInteractableInfo(44, EntityType::INTERACTABLE, -1, "LATCH", "An opened LATCH", "", false),
		EntityInfo::createItemInfo(45, EntityType::ITEM, 6, "WHISKEY", "A WHISKEY bottle", "The WHISKEY bottle is still sealed.", false, false),
		EntityInfo::createInteractableInfo(46, EntityType::INTERACTABLE, 6, "DRAWER", "A DRAWER that is very slightly opened but stuck", "You can see something shiny inside the DRAWER", false),
		EntityInfo::createInteractableInfo(47, EntityType::INTERACTABLE, -1, "DRAWER", "An opened DRAWER", "The DRAWER edge is slightly damaged.", false),
		EntityInfo::createItemInfo(48, EntityType::ITEM, -1, "KEY_A", "A small key labeled KEY_A", "", false, false),
		// Dining Room
		//EntityInfo::createExitInfo(49, EntityType::EXIT, 7, "Dining Room TO Main Hall", "A carved wooden door", Direction::E, false, "", 8),
		//EntityInfo::createExitInfo(50, EntityType::EXIT, 7, "Dining Room TO Kitchen", "A swing door", Direction::N, true, "The door is blocked by a big angry DOG.", 4),
		EntityInfo::createInteractableInfo(51, EntityType::INTERACTABLE, 7, "DOG", "A big angry DOG stands in front of the north door", "The DOG seems to be really, really angry.", true),
		EntityInfo::createInteractableInfo(52, EntityType::INTERACTABLE, -1, "DOG", "A big happy DOG lies down next to the north door", "The DOG seems to be really, really happy.", true),
		EntityInfo::createItemInfo(53, EntityType::ITEM, 7, "KEY_B", "A small key labeled KEY_B", "", false, false),
		EntityInfo::createItemInfo(54, EntityType::ITEM, 7, "BLUE_KEY", "A small BLUE_KEY", "", false, false),
		// Main Hall
		//EntityInfo::createExitInfo(55, EntityType::EXIT, 8, "Main Hall TO Dining Room", "A carved wooden door", Direction::W, false, "", 7),
		//EntityInfo::createExitInfo(56, EntityType::EXIT, 8, "Main Hall TO Trophy Room", "A nicely decorated door", Direction::E, false, "", 9),
		//EntityInfo::createExitInfo(57, EntityType::EXIT, 8, "Main Hall TO Empty Room", "A thick wooden door", Direction::N, true, "The door is locked.\nThere is a blue electronic screen next to the door displaying the word 'Closed'.", 5),
		//EntityInfo::createExitInfo(58, EntityType::EXIT, 8, "Main Hall TO END", "A heavy looking, tall door", Direction::S, true, "There is something keeping the door shut.", 8), // This exit will never actually be used.
		EntityInfo::createInteractableInfo(59, EntityType::INTERACTABLE, 8, "LOCK", "A triple LOCK on the south door", "The LOCK is labeled 'ABC'.", false),
		EntityInfo::createInteractableInfo(60, EntityType::INTERACTABLE, -1, "LOCK", "A triple LOCK on the south door, the first part is already opened", "The LOCK is labeled 'ABC', the first part is already opened.", false),
		EntityInfo::createInteractableInfo(61, EntityType::INTERACTABLE, -1, "LOCK", "A triple LOCK on the south door, only the third part remains locked", "The LOCK is labeled 'ABC', only the third part remains locked.", false),
		EntityInfo::createItemInfo(62, EntityType::ITEM, 8, "POTATO", "A beautiful POTATO", "The POTATO is very bright.", true, false),
		// Trophy Room
		//EntityInfo::createExitInfo(63, EntityType::EXIT, 9, "Trophy Room TO Main Hall", "A nicely decorated door", Direction::W, false, "", 8),
		//EntityInfo::createExitInfo(64, EntityType::EXIT, 9, "Trophy Room TO Office", "A red-painted door", Direction::N, true, "The door is locked from the other side.", 6),
		EntityInfo::createItemInfo(65, EntityType::ITEM, 9, "TROPHY", "A heavy-looking, mid-sized TROPHY", "The TROPHY has an unreadable inscription.", false, false),
		EntityInfo::createInteractableInfo(66, EntityType::INTERACTABLE, 9, "BAG", "A fabric BAG closed with a simple knot", "", false),
		EntityInfo::createInteractableInfo(67, EntityType::INTERACTABLE, -1, "BAG", "An opened fabric BAG", "", false),
		EntityInfo::createItemInfo(68, EntityType::ITEM, -1, "BONE", "A dog toy shaped like a BONE", "", false, false),
		EntityInfo::createItemInfo(69, EntityType::ITEM, -1, "BATTERIES", "A couple of standard BATTERIES", "The BATTERIES look brand new.", false, false),
		EntityInfo::createInteractableInfo(70, EntityType::INTERACTABLE, 9, "PANEL", "A small blue PANEL hangs from the wall", "The PANEL door is closed.", false),
		EntityInfo::createInteractableInfo(71, EntityType::INTERACTABLE, -1, "PANEL", "A small blue PANEL hangs from the wall", "The PANEL door is opened.", false),
		EntityInfo::createInteractableInfo(72, EntityType::INTERACTABLE, -1, "KEYHOLE", "A blue KEYHOLE in the back wall of the PANEL", "The KEYHOLE seems to be part of some electronic circuitry.", false)
	};
	for (EntityInfo& entityInfo : worldInfo)
	{
		entityFactory->createEntity(entityInfo);
	}

	// *** PLAYER *** //
	EntityInfo playerInfo = EntityInfo::createPlayerInfo(0, EntityType::PLAYER, -1, "", "", 2, 8);
	Player* player = static_cast<Player*>(entityFactory->createEntity(playerInfo));

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

	return player;
}
