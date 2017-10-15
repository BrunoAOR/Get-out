#include "World.h"

#include "globals.h"
#include "InputParser.h"
#include "NamesInfo.h"
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


World* world = nullptr;


World::World()
{
	m_inputParser = new InputParser();
	m_namesInfo = new NamesInfo();
}


World::~World()
{
	
}


LoopStatus World::init()
{
	// Create Player, Rooms, Exits, Items, Interactables, ItemUses, ItemWraps, InteractableOpens, Effects (children thereof)
	// Ensure Items' and interactables' names are added to the namesInfo list
	player = new Player(EntityType::PLAYER, "Jim", "A random human", 2);
	m_entities.push_back(player);

	Room* room = new Room(EntityType::ROOM, "Big Hall", "A big hall", false);
	m_entities.push_back(room);
	player->setParent(room);

	Item* item = new Item(EntityType::ITEM, "Key", "A small KEY", "The key has an engraving of a heart", false);
	m_entities.push_back(item);
	item->setParent(room);

	Interactable* interactable = new Interactable(EntityType::INTERACTABLE, "Lock", "A LOCK on the wall", "The lock has an engraving of a heart.", false, false);
	m_entities.push_back(interactable);
	interactable->setParent(room);

	Exit* exit = new Exit(EntityType::EXIT, "Big Hall north exit", "A metal door", Direction::N, false, "", nullptr);
	m_entities.push_back(exit);
	exit->setParent(room);

	// TODO: Replace these lines with actual object creation
	m_namesInfo->items.push_back("KEY");
	m_namesInfo->items.push_back("POTATO");

	m_namesInfo->interactables.push_back("KEYHOLE");
	m_namesInfo->interactables.push_back("POT");

	// After all the initialization, log the welcome message
	logWelcomeMessage();

	return LoopStatus::CONTINUE;
}


LoopStatus World::update(const std::string& userInput)
{
	if (userInput != "")
	{
		Instruction* instruction = m_inputParser->parse(userInput, *m_namesInfo);
		// TODO: Fully process the Instruction
		switch (instruction->actionType)
		{
		case ActionType::_UNDEFINED:
			OutputLog("ERROR: Resulting Instruction undefined!\n");
			return LoopStatus::UPDATE_ERROR;
			break;
		case ActionType::ERROR:
			consoleLog(instruction->errorDescription + "\n");
			break;
		case ActionType::QUIT:
			return LoopStatus::EXIT;
		case ActionType::HELP:
			consoleLog("Instruction for world as " + getStringFromAction(instruction->actionType) + "\n");
			logHelpMessage();
			break;
		case ActionType::LOOK:
		case ActionType::GO:
		case ActionType::TAKE:
		case ActionType::DROP:
		case ActionType::INSPECT:
		case ActionType::OPEN:
		case ActionType::USE:
		case ActionType::PUT:
			consoleLog("Instruction for player as " + getStringFromAction(instruction->actionType) + "\n");
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
	delete m_namesInfo;
	m_namesInfo = nullptr;
	delete m_inputParser;
	m_inputParser = nullptr;
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
		if (m_name == entity->getName())
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
		if (caselessEquals(iOpen->interactable->getName(), interactableName))
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


void World::logWelcomeMessage()
{
	// TODO: Polish the welcome message
	std::string message = "GET OUT\nA game based on the text adventure Zork\nDeveloped by Bruno Ortiz";
	message += "Use the HELP command to learn how to play\n";
	message += "--------------------------------------------------\n";
	message += "You find yourself in a dark room, uncertain of where you are or how you got there.\n";
	consoleLog(message);
}

void World::logHelpMessage()
{
	// TODO: Prepare the help message
	consoleLog("And this is where I would put my help message...\nIf I had one!\n");
}
