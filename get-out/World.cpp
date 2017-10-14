#include "World.h"

#include "globals.h"
#include "InputParser.h"
#include "NamesInfo.h"
#include "ActionType.h"
#include "Direction.h"
#include "Instruction.h"
#include "Entity.h"


World* world = nullptr;

World::World()
{
	inputParser = new InputParser();
	namesInfo = new NamesInfo();
}


World::~World()
{
	delete namesInfo;
	namesInfo = nullptr;
	delete inputParser;
	inputParser = nullptr;
	deleteCollection(entities);
	deleteCollection(interactableOpenCollection);
	deleteCollection(itemUseCollection);
	deleteCollection(itemPutCollection);
}


LoopStatus World::init()
{
	// Create Rooms, Exits, Items, Interactables, ItemUses, ItemWraps, InteractableOpens, Effects (children thereof)
	// Ensure Items' and interactables' names are added to the namesInfo list

	// TODO: Replace these lines with actual object creation
	namesInfo->items.push_back("KEY");
	namesInfo->items.push_back("POTATO");

	namesInfo->interactables.push_back("KEYHOLE");
	namesInfo->interactables.push_back("POT");

	// After all the initialization, log the welcome message (including mention of the HELP instruction)

	// TODO: Polish the welcome message
	consoleLog("GET OUT");
	consoleLog("A game based on the text adventure Zork");
	consoleLog("Developed by Bruno Ortiz");
	consoleLog("Use the HELP command to learn how to play");
	consoleLog('\n');
	consoleLog("--------------------------------------------------");
	consoleLog('\n');
	consoleLog("You find yourself in a dark room, uncertain of where you are or how you got there.");
	consoleLog('\n');

	return LoopStatus::CONTINUE;
}


LoopStatus World::update(const std::string& userInput)
{
	if (userInput != "")
	{
		consoleLog('\n');

		Instruction* instruction = inputParser->parse(userInput, *namesInfo);
		// TODO: Fully process the Instruction
		switch (instruction->actionType)
		{
		case ActionType::_UNDEFINED:
			OutputLog("ERROR: Resulting Instruction undefined!");
			return LoopStatus::UPDATE_ERROR;
			break;
		case ActionType::ERROR:
			consoleLog("Instruction rejected: " + instruction->errorDescription);
			break;
		case ActionType::QUIT:
		case ActionType::HELP:
			consoleLog("Instruction for world as " + getStringFromAction(instruction->actionType));
			break;
		case ActionType::LOOK:
		case ActionType::GO:
		case ActionType::TAKE:
		case ActionType::DROP:
		case ActionType::INSPECT:
		case ActionType::OPEN:
		case ActionType::USE:
		case ActionType::PUT:
			consoleLog("Instruction for player as " + getStringFromAction(instruction->actionType));
			break;
		default:
			break;
		}

		consoleLog('\n');
		// EVENTUALLY:
		delete instruction;
		instruction = nullptr;
	}
	return LoopStatus::CONTINUE;
}


LoopStatus World::close()
{
	return LoopStatus::EXIT;
}


Entity * World::getEntity(const std::string & name)
{
	for (Entity* entity : entities)
	{
		if (name == entity->getName())
		{
			return entity;
		}
	}

	return nullptr;
}


InteractableOpen * World::getInteractableOpen(const std::string & interactableName)
{
	// TODO: Implement getInteractableOpen
	return nullptr;
}


ItemUse * World::getItemUse(const std::string & itemName, const std::string & interactableName)
{
	// TODO: Implement getItemUse
	return nullptr;
}


ItemPut * World::getItemPut(const std::string & itemName, const std::string & containerItemName)
{
	// TODO: Implement getItemPut
	return nullptr;
}
