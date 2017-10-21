#include "World.h"

#include "globals.h"
#include "InputParser.h"
#include "InstructionType.h"
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
#include "EffectUnlockExit.h"


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

	Interactable* ant = new Interactable("ANT", "A giant angry ANT stands in front of the north door", "The ant seems to be really, really angry.", true);
	m_entities.push_back(ant);
	ant->setParent(room2);

	Interactable* happyAnt = new Interactable("ANT", "A giant happy ANT stands next to the north door", "The ant seems to be really, really happy.", true);
	m_entities.push_back(happyAnt);

	EffectReplaceEntity* antReplace = new EffectReplaceEntity("", ant, happyAnt);
	EffectUnlockExit* unlockEffect = new EffectUnlockExit("The any stepped aside.", lockedExit);
	ItemUse* antItemUse = new ItemUse("You feed the potato to the ant.", std::vector<ActionEffect*>{antReplace, unlockEffect}, true, item2, ant);
	m_actions.push_back(antItemUse);

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
	m_actions.push_back(cabinetInteractableOpen);

	EffectAddEntitiesToRoom* add2Effect = new EffectAddEntitiesToRoom("You see a metal rod inside the minicabinet.", std::vector<Entity*>{rodItem}, room2);
	EffectReplaceEntity* replace2Effect = new EffectReplaceEntity("", smallerPanel, smallerOpenedPanel);
	InteractableOpen* interactableOpen = new InteractableOpen("With a bit less force, you open the minicabinet doors.", std::vector<ActionEffect*>{add2Effect, replace2Effect}, true, smallerPanel);
	m_actions.push_back(interactableOpen);

	// After all the initialization, log the welcome message
	logWelcomeMessage();

	return LoopStatus::CONTINUE;
}


LoopStatus World::update(const std::string& userInput)
{
	LoopStatus loopStatus = LoopStatus::CONTINUE;
	if (userInput != "")
	{
		Instruction* instruction = m_inputParser->parse(userInput);
		// TODO: Fully process the Instruction
		switch (instruction->instructionType)
		{
		case InstructionType::_UNDEFINED:
			OutputLog("ERROR: Resulting Instruction undefined!\n");
			loopStatus = LoopStatus::UPDATE_ERROR;
			break;
		case InstructionType::ERROR:
			consoleLog(instruction->errorDescription);
			break;
		case InstructionType::QUIT:
			loopStatus = LoopStatus::EXIT;
			break;
		case InstructionType::HELP:
			logHelpMessage();
			break;
		case InstructionType::LOOK:
		case InstructionType::GO:
		case InstructionType::INVENTORY:
		case InstructionType::TAKE:
		case InstructionType::DROP:
		case InstructionType::INSPECT:
		case InstructionType::OPEN:
		case InstructionType::USE:
		case InstructionType::PUT:
			player->executeInstruction(instruction);
			break;
		default:
			break;
		}

		// EVENTUALLY:
		delete instruction;
		instruction = nullptr;
	}
	return loopStatus;
}


LoopStatus World::close()
{
	deleteCollection(m_entities);
	deleteCollection(m_actions);
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

Action * World::getAction(ActionType actionType, const std::string & firstEntityName, const std::string & secondEntityName)
{
	if (firstEntityName == "")
	{
		return nullptr;
	}

	for (auto action : m_actions)
	{
		if (action->getActionType() == actionType)
		{
			const Entity* firstEntity = action->getFirstEntity();
			const Entity* secondEntity = action->getSecondEntity();
			if (caselessEquals(firstEntity->getName(), firstEntityName)
				&& (secondEntityName == "" || secondEntity != nullptr && caselessEquals(secondEntity->getName(), secondEntityName)))
			{
				return action;
			}
		}
	}
	return nullptr;
}


void World::removeAction(Action * action)
{
	auto it = std::find(m_actions.begin(), m_actions.end(), action);
	assert(it != m_actions.end());
	m_actions.erase(it);
	delete action;
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
