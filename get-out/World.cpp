#include "World.h"

#include "globals.h"
#include "InputParser.h"
#include "ActionType.h"
#include "Direction.h"
#include "Instruction.h"
#include "Entity.h"
#include "InteractableOpen.h"
#include "ItemUse.h"
#include "ItemPut.h"
#include "Item.h"
#include "Interactable.h"

#include "EntityType.h"
#include "Player.h"
#include "Room.h"
#include "Exit.h"
#include "EffectAddEntitiesToRoom.h"
#include "EffectReplaceEntity.h"


World* world = nullptr;


World::World()
{
	m_inputParser = new InputParser();
}


World::~World()
{
	delete m_inputParser;
	m_inputParser = nullptr;
}


LoopStatus World::init()
{
	// Create Player, Rooms, Exits, Items, Interactables, ItemUses, ItemWraps, InteractableOpens, Effects (children thereof)
	// Ensure Items' and interactables' names are added to the namesInfo list

	// TODO: Create factory methods (or potentially a new class with said methods) to ensure setup is done properly.
	Room* room = new Room("Big Hall", "A big hall", false);
	m_entities.push_back(room);

	player = new Player("Jim", "A random human", 2, room);
	m_entities.push_back(player);

	Item* item = new Item("KEY", "A small KEY", "The key has an engraving of a heart", false);
	m_entities.push_back(item);
	item->setParent(room);

	Interactable* interactable = new Interactable("LOCK", "A LOCK on the wall", "The lock has an engraving of a heart", false);
	m_entities.push_back(interactable);
	interactable->setParent(room);

	// Second test room
	Room* room2 = new Room("Test room", "A room for testing purposes", false);
	m_entities.push_back(room2);

	Exit* exit = new Exit("Big Hall north exit", "A metal door", Direction::N, false, "", room2);
	m_entities.push_back(exit);
	exit->setParent(room);

	Exit* exit2 = new Exit("Test room south exit", "A rusted metal door", Direction::S, false, "", room);
	m_entities.push_back(exit2);
	exit2->setParent(room2);

	Exit* lockedExit = new Exit("Blocked door", "A normal looking door", Direction::N, true, "The door is blocked by a giant angry ant.", room);
	m_entities.push_back(lockedExit);
	lockedExit->setParent(room2);

	Item* item2 = new Item("POTATO", "A beautiful POTATO", "", false);
	m_entities.push_back(item2);
	item2->setParent(room2);

	Item* item3 = new Item("CARROT", "A small CARROT", "", false);
	m_entities.push_back(item3);
	item3->setParent(room2);

	Interactable* panelInteractable = new Interactable("CABINET", "A metal CABINET hangs from the wall", "The cabinet doors are closed.", false);
	m_entities.push_back(panelInteractable);
	panelInteractable->setParent(room2);

	Interactable* openedPanel = new Interactable("CABINET", "A metal CABINET hangs from the wall", "The cabinet doors are opened.", false);
	m_entities.push_back(openedPanel);
	Interactable* smallerPanel = new Interactable("MINICABINET", "A MINICABINET stuck to the back wall of the cabinet", "The doors are closed.", false);
	m_entities.push_back(smallerPanel);
	Interactable* smallerOpenedPanel = new Interactable("MINICABINET", "A MINICABINET stuck to the back wall of the cabinet", "The doors are opened.", false);
	m_entities.push_back(smallerOpenedPanel);

	Item* rodItem = new Item("ROD", "A metal ROD", "There are some unreadable markings on the rod.", false);
	m_entities.push_back(rodItem);

	EffectAddEntitiesToRoom* addEffect = new EffectAddEntitiesToRoom("You see a minicabinet inside the cabinet.", std::vector<Entity*>{smallerPanel}, room2);
	EffectReplaceEntity* replaceEffect = new EffectReplaceEntity("", panelInteractable, openedPanel);
	InteractableOpen* cabinetInteractableOpen = new InteractableOpen("With a bit of force, you open the cabinet doors.", std::vector<ActionEffect*>{addEffect, replaceEffect}, true, panelInteractable);;
	m_interactableOpenCollection.push_back(cabinetInteractableOpen);

	EffectAddEntitiesToRoom* add2Effect = new EffectAddEntitiesToRoom("You see a metal rod inside the minicabinet.", std::vector<Entity*>{rodItem}, room2);
	EffectReplaceEntity* replace2Effect = new EffectReplaceEntity("", smallerPanel, smallerOpenedPanel);
	InteractableOpen* interactableOpen = new InteractableOpen("With a bit less force, you open the minicabinet doors.", std::vector<ActionEffect*>{add2Effect, replace2Effect}, true, smallerPanel);
	m_interactableOpenCollection.push_back(interactableOpen);

	// After all the initialization, log the welcome message
	logWelcomeMessage();

	return LoopStatus::CONTINUE;
}


LoopStatus World::update(const std::string& userInput)
{
	if (userInput != "")
	{
		Instruction* instruction = m_inputParser->parse(userInput);
		// TODO: Fully process the Instruction
		switch (instruction->actionType)
		{
		case ActionType::_UNDEFINED:
			OutputLog("ERROR: Resulting Instruction undefined!\n");
			return LoopStatus::UPDATE_ERROR;
			break;
		case ActionType::ERROR:
			consoleLog(instruction->errorDescription);
			break;
		case ActionType::QUIT:
			return LoopStatus::EXIT;
		case ActionType::HELP:
			consoleLog("Instruction for world as " + getStringFromAction(instruction->actionType));
			logHelpMessage();
			break;
		case ActionType::LOOK:
		case ActionType::GO:
		case ActionType::INVENTORY:
		case ActionType::TAKE:
		case ActionType::DROP:
		case ActionType::INSPECT:
		case ActionType::OPEN:
		case ActionType::USE:
		case ActionType::PUT:
			player->executeInstruction(instruction);
			break;
		default:
			break;
		}

		// EVENTUALLY:
		delete instruction;
		instruction = nullptr;
	}
	return LoopStatus::CONTINUE;
}


LoopStatus World::close()
{
	deleteCollection(m_entities);
	deleteCollection(m_interactableOpenCollection);
	deleteCollection(m_itemUseCollection);
	deleteCollection(m_itemPutCollection);
	return LoopStatus::EXIT;
}


Entity * World::getEntity(const std::string & m_name)
{
	for (Entity* entity : m_entities)
	{
		if (caselessEquals(m_name, entity->getName()))
		{
			return entity;
		}
	}

	return nullptr;
}


InteractableOpen * World::getInteractableOpen(const std::string & interactableName)
{
	for (auto iOpen : m_interactableOpenCollection)
	{
		if (caselessEquals(iOpen->getInteractable()->getName(), interactableName))
		{
			return iOpen;
		}
	}
	return nullptr;
}


ItemUse * World::getItemUse(const std::string & itemName, const std::string & interactableName)
{
	for (auto iUse : m_itemUseCollection)
	{
		if (caselessEquals(iUse->item->getName(), itemName) && caselessEquals(iUse->interactable->getName(), interactableName))
		{
			return iUse;
		}
	}
	return nullptr;
}


ItemPut * World::getItemPut(const std::string & itemName, const std::string & containerItemName)
{
	for (auto iPut : m_itemPutCollection)
	{
		if (caselessEquals(iPut->item->getName(), itemName) && caselessEquals(iPut->container->getName(), containerItemName))
		{
			return iPut;
		}
	}
	return nullptr;
}


void World::removeInteractableOpen(InteractableOpen * interactableOpen)
{
		auto it = std::find(m_interactableOpenCollection.begin(), m_interactableOpenCollection.end(), interactableOpen);
		assert(it != m_interactableOpenCollection.end());
		m_interactableOpenCollection.erase(it);
		delete interactableOpen;
}


void World::logWelcomeMessage()
{
	// TODO: Polish the welcome message
	std::string message = "GET OUT\nA game based on the text adventure Zork\nDeveloped by Bruno Ortiz\n";
	message += "Use the HELP command to learn how to play\n";
	message += "--------------------------------------------------\n";
	message += "You find yourself in an unfamiliar hall, uncertain of where you are or how you got there.";
	consoleLog(message);
}


void World::logHelpMessage()
{
	// TODO: Prepare the help message
	consoleLog("And this is where I would put my help message...\nIf I had one!");
}
