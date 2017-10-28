#include "World.h"

#include "worldConfig.h"
#include "InputParser.h"
#include "EntityFactory.h"
#include "ActionFactory.h"
#include "Instruction.h"
#include "InstructionType.h"
#include "Player.h"
#include "globals.h"


World* world = nullptr;


World::World()
{
	m_inputParser = new InputParser();
	m_entityFactory = new EntityFactory();
	m_actionFactory = new ActionFactory(m_entityFactory);
}


World::~World()
{
	delete m_actionFactory;
	m_actionFactory = nullptr;
	delete m_entityFactory;
	m_entityFactory = nullptr;
	delete m_inputParser;
	m_inputParser = nullptr;
}


LoopStatus World::init()
{
	player = setUpWorld(m_entityFactory, m_actionFactory);
	// After all the initialization, log the welcome message
	consoleLog(getWelcomeMessage());

	return LoopStatus::CONTINUE;
}


LoopStatus World::update(const std::string& userInput)
{
	if (userInput != "")
	{
		Instruction* instruction = m_inputParser->parse(userInput);
		switch (instruction->instructionType)
		{
		case InstructionType::_UNDEFINED:
			OutputLog("ERROR: Resulting Instruction undefined!\n");
			m_loopStatus = LoopStatus::UPDATE_ERROR;
			break;
		case InstructionType::ERROR:
			consoleLog(instruction->errorDescription);
			break;
		case InstructionType::QUIT:
			m_loopStatus = LoopStatus::EXIT;
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

		delete instruction;
		instruction = nullptr;
	}
	return m_loopStatus;
}


LoopStatus World::close()
{
	m_actionFactory->close();
	m_entityFactory->close();
	return LoopStatus::EXIT;
}


Action* World::getAction(ActionType actionType, const Entity* firstEntity, const Entity* secondEntity) const
{
	return m_actionFactory->getAction(actionType, firstEntity, secondEntity);
}


void World::removeAction(Action * action) const
{
	m_actionFactory->removeAction(action);
}


void World::requestGameEnd()
{
	m_loopStatus = LoopStatus::EXIT;
}


void World::logHelpMessage()
{
	std::string message = "INFORMATION:\n";
	message += "The commands that you can perform in the game and their effects are the following:\n\n";
	message += "  HELP: Display this list of actions.\n";
	message += "  LOOK: Look around and get a description of the room you are currently in.\n";
	message += "  INVENTORY: Display a description of the items you are currently holding.\n";
	message += "  TAKE item: Attempt to pick up an item and put it in your inventory.\n";
	message += "  DROP item: Attempt to drop an item from your inventory.\n";
	message += "  INSPECT item: Display a detailed description of the item. (Example: INSPECT carrot)\n";
	message += "  OPEN item: Attempt to open an item. (Example: OPEN fridge)\n";
	message += "  USE item1 ON item2: Attempt to use the item1 on the item2. (Example : USE key ON lock)\n";
	message += "  PUT item1 IN item2: Attempt to place item1 within item2.\n";
	message += "                      Both items must be in your inventory for this action to succeed.\n";
	message += "\nDo note that commands and item names are not case-sensitive,\nso you can freely type everything in lowercase if you want.";
	consoleLog(message);
}
