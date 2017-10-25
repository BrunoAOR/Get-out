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
	logWelcomeMessage();

	return LoopStatus::CONTINUE;
}


LoopStatus World::update(const std::string& userInput)
{
	LoopStatus loopStatus = LoopStatus::CONTINUE;
	if (userInput != "")
	{
		Instruction* instruction = m_inputParser->parse(userInput);
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

		delete instruction;
		instruction = nullptr;
	}
	return loopStatus;
}


LoopStatus World::close()
{
	m_actionFactory->close();
	m_entityFactory->close();
	return LoopStatus::EXIT;
}


Entity * World::getEntity(const std::string & name)
{
	return m_entityFactory->getEntity(name);
}

Action * World::getAction(ActionType actionType, const std::string & firstEntityName, const std::string & secondEntityName)
{
	return m_actionFactory->getAction(actionType, firstEntityName, secondEntityName);
}


void World::removeAction(Action * action)
{
	m_actionFactory->removeAction(action);
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
