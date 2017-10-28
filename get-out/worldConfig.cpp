#include "worldConfig.h"
#include "Player.h"
#include "EntityFactory.h"
#include "ActionFactory.h"
#include "EntityInfo.h"
#include "EntityType.h"
#include "Direction.h"
#include "ActionType.h"

#include "Item.h"
#include "Exit.h"
#include "Room.h"

#include "EffectAddEntitiesToRoom.h"
#include "EffectReplaceEntity.h"
#include "EffectUnlockExit.h"
#include "EffectPlaceItemInItem.h"
#include "EffectRemoveEntities.h"
#include "EffectGameEnd.h"


Player* setUpTestWorld(EntityFactory* entityFactory, ActionFactory* actionFactory)
{
	std::vector<EntityInfo> worldInfo{
		// Rooms
		EntityInfo::createRoomInfo(1, EntityType::ROOM, -1, "Big Hall", "A big hall", false),
		EntityInfo::createRoomInfo(2, EntityType::ROOM, -1, "Test room", "A room for testing purposes", true),
		// Room 1
		EntityInfo::createExitInfo(3, EntityType::EXIT, 1, "Big Hall north exit", "A metal door", Direction::N, false, "", 2),
		EntityInfo::createItemInfo(4, EntityType::ITEM, 1, "KEY", "A small KEY", "The KEY has an engraving of a heart.", false),
		EntityInfo::createItemInfo(5, EntityType::ITEM, 1, "KEYCHAIN", "A shiny KEYCHAIN", "", true),
		EntityInfo::createItemInfo(6, EntityType::ITEM, 1, "FLASHLIGHT", "A small but powerful led FLASHLIGHT", "The FLASHLIGHT looks brand new.", true, true),
		EntityInfo::createInteractableInfo(7, EntityType::INTERACTABLE, 1, "LOCK", "A LOCK on the wall", "The LOCK has an engraving of a heart.", false),
		// Room 2
		EntityInfo::createExitInfo(8, EntityType::EXIT, 2, "Test room south exit", "A rusted metal door", Direction::S, false, "", 1),
		EntityInfo::createExitInfo(9, EntityType::EXIT, 2, "Blocked door", "A normal looking door", Direction::N, true, "The door is blocked by a giant angry ANT.", 1),
		EntityInfo::createItemInfo(10, EntityType::ITEM, 2, "POTATO", "A beautiful POTATO", "", false),
		EntityInfo::createItemInfo(11, EntityType::ITEM, 2, "CARROT", "A small CARROT", "", false),
		EntityInfo::createInteractableInfo(12, EntityType::INTERACTABLE, 2, "ANT", "A giant angry ANT stands in front of the north door", "The ANT seems to be really, really angry.", true),
		EntityInfo::createInteractableInfo(13, EntityType::INTERACTABLE, -1, "ANT", "A giant happy ANT stands next to the north door", "The ANT seems to be really, really happy.", true),
		EntityInfo::createInteractableInfo(14, EntityType::INTERACTABLE, 2, "CABINET", "A metal CABINET hangs from the wall", "The CABINET doors are closed.", false),
		EntityInfo::createInteractableInfo(15, EntityType::INTERACTABLE, -1, "CABINET", "A metal CABINET hangs from the wall", "The CABINET doors are opened.", false),
		EntityInfo::createInteractableInfo(16, EntityType::INTERACTABLE, -1, "MINICABINET", "A MINICABINET stuck to the back wall of the cabinet", "The doors on the MINICABINET are closed.", false),
		EntityInfo::createInteractableInfo(17, EntityType::INTERACTABLE, -1, "MINICABINET", "A MINICABINET stuck to the back wall of the cabinet", "The doors on the MINICABINET are opened.", false),
		EntityInfo::createItemInfo(18, EntityType::ITEM, -1, "ROD", "A metal ROD", "There are some unreadable markings on the ROD.", false),

	};

	for (EntityInfo& entityInfo : worldInfo)
	{
		entityFactory->createEntity(entityInfo);
	}

	EntityInfo playerInfo = EntityInfo::createPlayerInfo(0, EntityType::PLAYER, -1, "Jim", "A random human", 2, 1);
	Player* player = static_cast<Player*>(entityFactory->createEntity(playerInfo));

	EffectGameEnd* endGameEffect = new EffectGameEnd(getGameEndMessage());
	Action* keyOnLock = actionFactory->createAction(ActionType::ItemUse, "You use the key on the lock and are finally able to leave the house.", std::vector<ActionEffect*>{endGameEffect}, true, 4, 7);

	EffectPlaceItemInItem* keyInKeychain = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(4)), static_cast<Item*>(entityFactory->getEntity(5)));
	Action* keyInKeychainPut = actionFactory->createAction(ActionType::ItemPut, "You placed the KEY in the KEYCHAIN", std::vector<ActionEffect*>{keyInKeychain}, true, 4, 5);

	EffectReplaceEntity* antReplace = new EffectReplaceEntity("", entityFactory->getEntity(12), entityFactory->getEntity(13));
	EffectUnlockExit* unlockEffect = new EffectUnlockExit("The ANT stepped aside.", static_cast<Exit*>(entityFactory->getEntity(9)));
	EffectRemoveEntities* removeEffect = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(10)});
	Action* antItemUse = actionFactory->createAction(ActionType::ItemUse, "You feed the POTATO to the ANT.", std::vector<ActionEffect*>{antReplace, unlockEffect, removeEffect}, true, 10, 12);

	EffectReplaceEntity* replaceEffect = new EffectReplaceEntity("", entityFactory->getEntity(14), entityFactory->getEntity(15));
	EffectAddEntitiesToRoom* addEffect = new EffectAddEntitiesToRoom("You see a MINICABINET inside the CABINET.", std::vector<Entity*>{entityFactory->getEntity(16)}, static_cast<Room*>(entityFactory->getEntity(2)));
	Action* cabinetInteractableOpen = actionFactory->createAction(ActionType::InteractableOpen, "With a bit of force, you open the CABINET doors.", std::vector<ActionEffect*>{replaceEffect, addEffect}, true, 14);

	EffectReplaceEntity* replace2Effect = new EffectReplaceEntity("", entityFactory->getEntity(16), entityFactory->getEntity(17));
	EffectAddEntitiesToRoom* add2Effect = new EffectAddEntitiesToRoom("You see a metal ROD inside the MINICABINET.", std::vector<Entity*>{entityFactory->getEntity(18)}, static_cast<Room*>(entityFactory->getEntity(2)));
	Action* interactableOpen = actionFactory->createAction(ActionType::InteractableOpen, "With a bit less force, you open the MINICABINET doors.", std::vector<ActionEffect*>{replace2Effect, add2Effect}, true, 16);

	return player;
}


Player* setUpWorld(EntityFactory* entityFactory, ActionFactory* actionFactory)
{

	// *** ENTITIES *** //

	std::vector<EntityInfo> worldInfo{
		// Rooms
		EntityInfo::createRoomInfo(1, EntityType::ROOM, -1, "Storage Room", "A small storage room", true),
		EntityInfo::createRoomInfo(2, EntityType::ROOM, -1, "Garden", "A fairly small garden with beautiful roses", false),
		EntityInfo::createRoomInfo(3, EntityType::ROOM, -1, "Tool Shed", "A dirty shed", false),
		EntityInfo::createRoomInfo(4, EntityType::ROOM, -1, "Kitchen", "A very clean, but fairly empty kitchen", false),
		EntityInfo::createRoomInfo(5, EntityType::ROOM, -1, "Empty Room", "An empty room with nothing special in it", false),
		EntityInfo::createRoomInfo(6, EntityType::ROOM, -1, "Office", "A nice office with fancy looking wooden walls", false),
		EntityInfo::createRoomInfo(7, EntityType::ROOM, -1, "Dining Room", "A room with a long dining table in the middle", true),
		EntityInfo::createRoomInfo(8, EntityType::ROOM, -1, "Main Hall", "A big hall without much to see in it", true),
		EntityInfo::createRoomInfo(9, EntityType::ROOM, -1, "Trophy Room", "A room that contains prizes and trophies on the walls", false),
		// Storage Room
		EntityInfo::createExitInfo(10, EntityType::EXIT, 1, "Storage Room TO Garden", "A rusted metal door", Direction::E, false, "", 2),
		EntityInfo::createInteractableInfo(11, EntityType::INTERACTABLE, 1, "CABINET", "A metal CABINET hangs from the wall", "The CABINET doors are closed.", false),
		EntityInfo::createInteractableInfo(12, EntityType::INTERACTABLE, -1, "CABINET", "A metal CABINET hangs from the wall", "The CABINET doors are opened.", false),
		EntityInfo::createItemInfo(13, EntityType::ITEM, -1, "KEYCHAIN", "A shiny KEYCHAIN", "", false, false),
		EntityInfo::createItemInfo(14, EntityType::ITEM, 13, "KEY_C", "A small key labeled KEY_C", "", false, false),
		// Garden
		EntityInfo::createExitInfo(15, EntityType::EXIT, 2, "Garden TO Storage Room", "A rusted metal door", Direction::W, true, "The MAN standing in front of the door pushes you away.", 1),
		EntityInfo::createExitInfo(16, EntityType::EXIT, 2, "Garden TO Tool Shed", "A wooden door covered in moss", Direction::E, false, "", 3),
		EntityInfo::createExitInfo(17, EntityType::EXIT, 2, "Garden TO Empty Room", "A wooden door with small glass panels on the top half", Direction::S, false, "", 5),
		EntityInfo::createInteractableInfo(18, EntityType::INTERACTABLE, 2, "MAN", "An old MAN stands in from of the western exit", "The MAN doesn't seem to be going anywhere.", false),
		EntityInfo::createInteractableInfo(19, EntityType::INTERACTABLE, -1, "MAN", "The old MAN is still standing in front of the door", "The MAN is trying to clear the flour from his eyes.", false),
		EntityInfo::createInteractableInfo(20, EntityType::INTERACTABLE, -1, "MAN", "An old MAN lying on the ground", "The MAN seems to be unconscious.", false),
		// Tool Shed
		EntityInfo::createExitInfo(21, EntityType::EXIT, 3, "Tool Shed TO Garden", "A wooden door covered in moss", Direction::W, false, "", 2),
		EntityInfo::createExitInfo(22, EntityType::EXIT, 3, "Tool Shed TO Office", "A fancy looking door", Direction::S, false, "", 6),
		EntityInfo::createItemInfo(23, EntityType::ITEM, 3, "SHOVEL", "A small, worn out SHOVEL", "", false, false),
		EntityInfo::createItemInfo(24, EntityType::ITEM, 3, "SCREWDRIVER", "A standard Phillips SCREWDRIVER", "", false, false),
		// Kitchen
		EntityInfo::createExitInfo(25, EntityType::EXIT, 4, "Kitchen TO Empty Room", "A heavy looking wooden door", Direction::E, true, "The door is bolted shut.", 5),
		EntityInfo::createExitInfo(26, EntityType::EXIT, 4, "Kitchen TO Dining Room", "A swing door", Direction::S, false, "", 7),
		EntityInfo::createInteractableInfo(27, EntityType::INTERACTABLE, 4, "CUPBOARD", "A large CUPBOARD with glass doors", "The CUPBOARD doors are closed shut but you can see a FLASHLIGHT and a bag of FLOUR inside.", false),
		EntityInfo::createInteractableInfo(28, EntityType::INTERACTABLE, -1, "CUPBOARD", "A large CUPBOARD with broken glass doors", "The CUPBOARD doors are still closed but the glass panes are broken.", false),
		EntityInfo::createItemInfo(29, EntityType::ITEM, -1, "FLASHLIGHT", "A small but powerful led FLASHLIGHT", "The FLASHLIGHT looks brand new.", true, true),
		EntityInfo::createItemInfo(30, EntityType::ITEM, -1, "FLOUR", "A bag of FLOUR", "", false, false),
		EntityInfo::createInteractableInfo(31, EntityType::INTERACTABLE, 4, "DRAWERS", "A group of DRAWERS next to the sink.", "The DRAWERS are closed.", false),
		EntityInfo::createInteractableInfo(32, EntityType::INTERACTABLE, -1, "DRAWERS", "A group of DRAWERS next to the sink.", "The DRAWERS are opened.", false),
		EntityInfo::createItemInfo(33, EntityType::ITEM, -1, "PAN", "A big PAN", "", false, false),
		EntityInfo::createItemInfo(34, EntityType::ITEM, -1, "POT", "A very small POT", "", false, false),
		// Empty Room
		EntityInfo::createExitInfo(35, EntityType::EXIT, 5, "Empty Room TO Kitchen", "A heavy looking wooden door", Direction::W, true, "The door is bolted shut.", 4),
		EntityInfo::createExitInfo(36, EntityType::EXIT, 5, "Empty Room TO Garden", "A wooden door with small glass panels on the top half", Direction::N, true, "When trying to approach the door, the RAT attemps to bite you so you step back.", 2),
		EntityInfo::createExitInfo(37, EntityType::EXIT, 5, "Empty Room TO Main Hall", "A double wooden door", Direction::S, false, "", 8),
		EntityInfo::createInteractableInfo(38, EntityType::INTERACTABLE, 5, "RAT", "A big RAT rests in front of the northern door.", "The RAT doesn't look very friendly.", false),
		EntityInfo::createItemInfo(39, EntityType::ITEM, 5, "BALL", "A small stress BALL", "", false, false),
		// Office
		EntityInfo::createExitInfo(40, EntityType::EXIT, 6, "Office TO Tool shed", "A fancy looking door", Direction::N, false, "", 3),
		EntityInfo::createExitInfo(41, EntityType::EXIT, 6, "Office TO Trophy Room", "A fancy looking door", Direction::S, true, "The door is locked by a LATCH", 9),
		EntityInfo::createInteractableInfo(42, EntityType::INTERACTABLE, 6, "LATCH", "A LATCH keeping the southern door locked", "", false),
		EntityInfo::createInteractableInfo(43, EntityType::INTERACTABLE, -1, "LATCH", "An opened LATCH", "", false),
		EntityInfo::createItemInfo(44, EntityType::ITEM, 6, "WHISKEY", "A blue-label WHISKEY bottle", "The WHISKEY bottle is still sealed.", false, false),
		EntityInfo::createInteractableInfo(45, EntityType::INTERACTABLE, 6, "DRAWER", "A DRAWER that is very slightly opened but stuck", "You can see something shiny inside the DRAWER", false),
		EntityInfo::createInteractableInfo(46, EntityType::INTERACTABLE, -1, "DRAWER", "An opened DRAWER", "The DRAWER edge is slightly damaged.", false),
		EntityInfo::createItemInfo(47, EntityType::ITEM, -1, "KEY_A", "A small key labeled KEY_A", "", false, false),
		// Dining Room
		EntityInfo::createExitInfo(48, EntityType::EXIT, 7, "Dining Room TO Main Hall", "A nicely decorated door", Direction::E, false, "", 8),
		EntityInfo::createExitInfo(49, EntityType::EXIT, 7, "Dining Room TO Kitchen", "A swing door", Direction::N, true, "The door is blocked by a big angry DOG.", 4),
		EntityInfo::createInteractableInfo(50, EntityType::INTERACTABLE, 7, "DOG", "A big angry DOG stands in front of the north door", "The DOG seems to be really, really angry.", true),
		EntityInfo::createInteractableInfo(51, EntityType::INTERACTABLE, -1, "DOG", "A big happy DOG lies down next to the north door", "The DOG seems to be really, really happy.", true),
		EntityInfo::createItemInfo(52, EntityType::ITEM, 7, "KEY_B", "A small key labeled KEY_B", "", false, false),
		EntityInfo::createItemInfo(53, EntityType::ITEM, 7, "BLUE_KEY", "A small BLUE_KEY", "", false, false),
		// Main Hall
		EntityInfo::createExitInfo(54, EntityType::EXIT, 8, "Main Hall TO Dining Room", "A nicely decorated door", Direction::W, false, "", 7),
		EntityInfo::createExitInfo(55, EntityType::EXIT, 8, "Main Hall TO Trophy Room", "A nicely decorated door", Direction::E, false, "", 9),
		EntityInfo::createExitInfo(56, EntityType::EXIT, 8, "Main Hall TO Empty Room", "A double wooden door", Direction::N, true, "The door is locked.\nThere is a blue electronic screen next to the door displaying the word 'Closed'.", 5),
		EntityInfo::createExitInfo(57, EntityType::EXIT, 8, "Main Hall TO END", "A heavy looking, tall door", Direction::S, true, "There is something keeping the door shut.", 8), // This exit will never actually be used.
		EntityInfo::createInteractableInfo(58, EntityType::INTERACTABLE, 8, "LOCK", "A triple LOCK on the south door", "The LOCK is labeled 'ABC'.", false),
		EntityInfo::createInteractableInfo(59, EntityType::INTERACTABLE, -1, "LOCK", "A triple LOCK on the south door, the first part is already opened", "The LOCK is labeled 'ABC', the first part is already opened.", false),
		EntityInfo::createInteractableInfo(60, EntityType::INTERACTABLE, -1, "LOCK", "A triple LOCK on the south door, only the third part remains locked", "The LOCK is labeled 'ABC', only the third part remains locked.", false),
		EntityInfo::createItemInfo(61, EntityType::ITEM, 8, "POTATO", "A beautiful POTATO", "The POTATO is very bright.", true, false),
		// Trophy Room
		EntityInfo::createExitInfo(62, EntityType::EXIT, 9, "Trophy Room TO Main Hall", "A nicely decorated door", Direction::W, false, "", 8),
		EntityInfo::createExitInfo(63, EntityType::EXIT, 9, "Trophy Room TO Office", "A fancy looking door", Direction::N, true, "The door is locked from the other side.", 6),
		EntityInfo::createItemInfo(64, EntityType::ITEM, 9, "TROPHY", "A heavy-looking, mid-sized TROPHY", "The TROPHY has an unreadable inscription.", false, false),
		EntityInfo::createInteractableInfo(65, EntityType::INTERACTABLE, 9, "BAG", "A fabric BAG closed with a simple knot", "", false),
		EntityInfo::createInteractableInfo(66, EntityType::INTERACTABLE, -1, "BAG", "An opened fabric BAG", "", false),
		EntityInfo::createItemInfo(67, EntityType::ITEM, -1, "BONE", "A dog toy shaped like a BONE", "", false, false),
		EntityInfo::createInteractableInfo(68, EntityType::INTERACTABLE, 9, "PANEL", "A small blue PANEL hangs from the wall", "The PANEL door is closed.", false),
		EntityInfo::createInteractableInfo(69, EntityType::INTERACTABLE, -1, "PANEL", "A small blue PANEL hangs from the wall", "The PANEL door is opened.", false),
		EntityInfo::createInteractableInfo(70, EntityType::INTERACTABLE, -1, "KEYHOLE", "A blue KEYHOLE in the back wall of the PANEL", "The KEYHOLE seems to be part of some electronic circuitry.", false),
		

	};
	for (EntityInfo& entityInfo : worldInfo)
	{
		entityFactory->createEntity(entityInfo);
	}
	
	// *** PLAYER *** //
	EntityInfo playerInfo = EntityInfo::createPlayerInfo(0, EntityType::PLAYER, -1, "", "", 2, 8);
	Player* player = static_cast<Player*>(entityFactory->createEntity(playerInfo));

	// *** ACTIONS *** //

	// Storage Room
	{
		EffectReplaceEntity* cabinetReplace = new EffectReplaceEntity("", entityFactory->getEntity(11), entityFactory->getEntity(12));
		EffectAddEntitiesToRoom* keychainAdd = new EffectAddEntitiesToRoom("You see a KEYCHAIN inside the CABINET.", std::vector<Entity*>{entityFactory->getEntity(13)}, static_cast<Room*>(entityFactory->getEntity(1)));
		actionFactory->createAction(ActionType::InteractableOpen, "With a bit of force, you open the CABINET doors.", std::vector<ActionEffect*>{cabinetReplace, keychainAdd}, true, 11);
	}

	// Garden
	{
		EffectReplaceEntity* man1Replace = new EffectReplaceEntity("The MAN remains standing but is blinded and trying to clear the FLOUR out of his eyes.", entityFactory->getEntity(18), entityFactory->getEntity(19));
		EffectRemoveEntities* flourRemove = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(30)});
		actionFactory->createAction(ActionType::ItemUse, "You throw a handfull of FLOUR towards the MAN's face.", std::vector<ActionEffect*>{man1Replace, flourRemove}, true, 30, 18);

		EffectReplaceEntity* man2Replace = new EffectReplaceEntity("", entityFactory->getEntity(19), entityFactory->getEntity(20));
		EffectUnlockExit* unlockGardenToStorageRoom = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(15)));
		actionFactory->createAction(ActionType::ItemUse, "You use the SHOVEL to hit the MAN on the head and he falls to the ground.", std::vector<ActionEffect*>{man2Replace, unlockGardenToStorageRoom}, true, 23, 19);
	}

	// Kitchen
	{
		EffectReplaceEntity* cupboardReplace = new EffectReplaceEntity("", entityFactory->getEntity(27), entityFactory->getEntity(28));
		EffectAddEntitiesToRoom* flashlightAndFlourAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(29), entityFactory->getEntity(30)}, static_cast<Room*>(entityFactory->getEntity(4)));
		actionFactory->createAction(ActionType::ItemUse, "You swing the TROPHY at the CUPBOARD glass doors and they shatter.", std::vector<ActionEffect*>{cupboardReplace, flashlightAndFlourAdd}, true, 64, 27);

		EffectReplaceEntity* drawersReplace = new EffectReplaceEntity("", entityFactory->getEntity(31), entityFactory->getEntity(32));
		EffectAddEntitiesToRoom* panAndPotAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(33), entityFactory->getEntity(34)}, static_cast<Room*>(entityFactory->getEntity(4)));
		actionFactory->createAction(ActionType::InteractableOpen, "You open all of the DRAWERS and only find a big PAN and a very small POT.", std::vector<ActionEffect*>{drawersReplace, panAndPotAdd}, true, 31);
	}

	// Empty Room
	{
		EffectRemoveEntities* ratRemove = new EffectRemoveEntities("The RAT runs away through a gap on the door.", std::vector<Entity*>{entityFactory->getEntity(38)});
		EffectAddEntitiesToRoom* panFromInvToRoomAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(33)}, static_cast<Room*>(entityFactory->getEntity(5)));
		EffectUnlockExit* unlockEmptyRoomToGarden = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(36)));
		actionFactory->createAction(ActionType::ItemUse, "You throw the PAN at the RAT.", std::vector<ActionEffect*>{ratRemove, panFromInvToRoomAdd, unlockEmptyRoomToGarden}, true, 33, 38);
	}

	// Office
	{
		EffectReplaceEntity* latchReplace = new EffectReplaceEntity("", entityFactory->getEntity(42), entityFactory->getEntity(43));
		EffectUnlockExit* unlockOfficeToTrophyRoom = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(41)));
		EffectUnlockExit* unlockTrophyRoomToOffice = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(63)));
		actionFactory->createAction(ActionType::InteractableOpen, "You pull on the LATCH to open it.", std::vector<ActionEffect*>{latchReplace, unlockOfficeToTrophyRoom, unlockTrophyRoomToOffice}, true, 42);

		EffectReplaceEntity* drawerReplace = new EffectReplaceEntity("", entityFactory->getEntity(45), entityFactory->getEntity(46));
		EffectAddEntitiesToRoom* keyAAdd = new EffectAddEntitiesToRoom("You can see a small key labeled KEY_A", std::vector<Entity*>{entityFactory->getEntity(47)}, static_cast<Room*>(entityFactory->getEntity(6)));
		actionFactory->createAction(ActionType::ItemUse, "You use the SCREWDRIVER to force the DRAWER open.", std::vector<ActionEffect*>{drawerReplace, keyAAdd}, true, 24, 45);

		EffectPlaceItemInItem* keyAInKeychainPlace = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(47)), static_cast<Item*>(entityFactory->getEntity(13)));
		actionFactory->createAction(ActionType::ItemPut, "You placed the KEY_A in the KEYCHAIN", std::vector<ActionEffect*>{keyAInKeychainPlace}, true, 47, 13);

	}

	// Dining Room
	{
		EffectReplaceEntity* dogReplace = new EffectReplaceEntity("The DOG stops being angry and moves aside with the BONE in his mouth.", entityFactory->getEntity(50), entityFactory->getEntity(51));
		EffectRemoveEntities* boneRemove = new EffectRemoveEntities("", std::vector<Entity*>{entityFactory->getEntity(67)});
		EffectUnlockExit* unlockDiningRoomToKitchen = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(49)));
		actionFactory->createAction(ActionType::ItemUse, "Slowly, you give the BONE to the big DOG.", std::vector<ActionEffect*>{dogReplace, boneRemove, unlockDiningRoomToKitchen}, true, 67, 50);

		EffectPlaceItemInItem* keyBInKeychainPlace = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(52)), static_cast<Item*>(entityFactory->getEntity(13)));
		actionFactory->createAction(ActionType::ItemPut, "You placed the KEY_B in the KEYCHAIN", std::vector<ActionEffect*>{keyBInKeychainPlace}, true, 52, 13);
	}

	// Main Hall
	{
		EffectReplaceEntity* lock1Replace = new EffectReplaceEntity("", entityFactory->getEntity(58), entityFactory->getEntity(59));
		actionFactory->createAction(ActionType::ItemUse, "You use the KEY_A to open the first part of the triple LOCK.", std::vector<ActionEffect*>{lock1Replace}, true, 47, 58);

		EffectReplaceEntity* lock2Replace = new EffectReplaceEntity("", entityFactory->getEntity(59), entityFactory->getEntity(60));
		actionFactory->createAction(ActionType::ItemUse, "You use the KEY_B to open the second part of the triple LOCK.", std::vector<ActionEffect*>{lock2Replace}, true, 52, 59);

		EffectGameEnd* gameEnd = new EffectGameEnd(getGameEndMessage());
		actionFactory->createAction(ActionType::ItemUse, "You use the KEY_C to open the final part of the triple LOCK.", std::vector<ActionEffect*>{gameEnd}, true, 14, 60);
	}

	// Trophy Room
	{
		EffectReplaceEntity* bagReplace = new EffectReplaceEntity("", entityFactory->getEntity(65), entityFactory->getEntity(66));
		EffectAddEntitiesToRoom* boneAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(67)}, static_cast<Room*>(entityFactory->getEntity(9)));
		actionFactory->createAction(ActionType::InteractableOpen, "You undo the knot, open the BAG and find a BONE inside.", std::vector<ActionEffect*>{bagReplace, boneAdd}, true, 65);

		EffectReplaceEntity* panelReplace = new EffectReplaceEntity("", entityFactory->getEntity(68), entityFactory->getEntity(69));
		EffectAddEntitiesToRoom* keyholeAdd = new EffectAddEntitiesToRoom("", std::vector<Entity*>{entityFactory->getEntity(70)}, static_cast<Room*>(entityFactory->getEntity(9)));
		actionFactory->createAction(ActionType::InteractableOpen, "You open the PANEL door and see a blue KEYHOLE in the back wall.", std::vector<ActionEffect*>{panelReplace, keyholeAdd}, true, 68);

		EffectUnlockExit* unlockMainHallToEmptyRoom = new EffectUnlockExit("", static_cast<Exit*>(entityFactory->getEntity(56)));
		actionFactory->createAction(ActionType::ItemUse, "You use the BLUE_KEY on the blue KEYHOLE and hear a loud clicking noise coming from elsewhere.", std::vector<ActionEffect*>{unlockMainHallToEmptyRoom}, true, 53, 70);

	}

	return player;
}


std::string getWelcomeMessage()
{
	// TODO: Polish the welcome message
	std::string message = "GET OUT\n";
	message += "Developed by Bruno Ortiz\n";
	message += "A game based on the text adventure Zork\n";
	message += "Use the HELP command to learn how to play\n";
	message += "--------------------------------------------------\n";
	message += "You find yourself in an unfamiliar place, uncertain of where you are or how you got there.\n";
	message += "You can see a door to the south end of the room and hear some noices coming from the other side.";
	return message;
}


std::string getGameEndMessage()
{
	std::string message = "\n\n";
	message += "As you remove the heavy triple lock from the door, you can hear footsteps on the other side.\n";
	message += "Quickly, you open the door and are blinded by a very bright white light.\n";
	message += "While trying to adjust to the extreme lighting, you feel a needle in your neck.\n";
	message += "As you fall down to the ground, fainting, you hear a rather high-pitched voice saying:\n";
	message += "'THE EXIT IS A LIE'\n\n";
	message += "Well, you finished the game.\n";
	message += "It's probably not the ending you expected, but at least you have some closure now.";
	return message;
}
