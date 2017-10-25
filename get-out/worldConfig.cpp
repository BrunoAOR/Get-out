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
#include "EffectConsumeItem.h"
#include "EffectPlaceItemInItem.h"

Player* setUpWorld(EntityFactory* entityFactory, ActionFactory* actionFactory)
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
		EntityInfo::createExitInfo(9, EntityType::EXIT, 2, "Blocked door", "A normal looking door", Direction::N, true, "The door is blocked by a giant angry ant.", 1),
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

	EffectPlaceItemInItem* keyInKeychain = new EffectPlaceItemInItem("", static_cast<Item*>(entityFactory->getEntity(4)), static_cast<Item*>(entityFactory->getEntity(5)));
	Action* keyInKeychainPut = actionFactory->createAction(ActionType::ItemPut, "You placed the KEY in the KEYCHAIN", std::vector<ActionEffect*>{keyInKeychain}, true, 4, 5);

	EffectReplaceEntity* antReplace = new EffectReplaceEntity("", entityFactory->getEntity(12), entityFactory->getEntity(13));
	EffectUnlockExit* unlockEffect = new EffectUnlockExit("The ANT stepped aside.", static_cast<Exit*>(entityFactory->getEntity(9)));
	EffectConsumeItem* consumeEffect = new EffectConsumeItem("", static_cast<Item*>(entityFactory->getEntity(10)));
	Action* antItemUse = actionFactory->createAction(ActionType::ItemUse, "You feed the POTATO to the ANT.", std::vector<ActionEffect*>{antReplace, unlockEffect, consumeEffect}, true, 10, 12);

	EffectReplaceEntity* replaceEffect = new EffectReplaceEntity("", entityFactory->getEntity(14), entityFactory->getEntity(14));
	EffectAddEntitiesToRoom* addEffect = new EffectAddEntitiesToRoom("You see a MINICABINET inside the CABINET.", std::vector<Entity*>{entityFactory->getEntity(16)}, static_cast<Room*>(entityFactory->getEntity(2)));
	Action* cabinetInteractableOpen = actionFactory->createAction(ActionType::InteractableOpen, "With a bit of force, you open the CABINET doors.", std::vector<ActionEffect*>{replaceEffect, addEffect}, true, 14);

	EffectReplaceEntity* replace2Effect = new EffectReplaceEntity("", entityFactory->getEntity(16), entityFactory->getEntity(17));
	EffectAddEntitiesToRoom* add2Effect = new EffectAddEntitiesToRoom("You see a metal ROD inside the MINICABINET.", std::vector<Entity*>{entityFactory->getEntity(18)}, static_cast<Room*>(entityFactory->getEntity(2)));
	Action* interactableOpen = actionFactory->createAction(ActionType::InteractableOpen, "With a bit less force, you open the MINICABINET doors.", std::vector<ActionEffect*>{replace2Effect, add2Effect}, true, 16);

	return player;
}
