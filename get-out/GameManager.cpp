#include "GameManager.h"

#include "ActionFactory.h"
#include "EntityFactory.h"
#include "GameDataLoader.h"
#include "InputParser.h"
#include "InputReader.h"
#include "Instruction.h"
#include "InstructionType.h"
#include "Player.h"


GameManager::GameManager()
{
	m_inputReader = new InputReader();
	m_inputParser = new InputParser();
	m_entityFactory = new EntityFactory();
	m_actionFactory = new ActionFactory(m_entityFactory);
	m_gameDataLoader = new GameDataLoader(m_entityFactory, m_actionFactory);
}


GameManager::~GameManager()
{
	delete m_gameDataLoader;
	m_gameDataLoader = nullptr; 
	delete m_actionFactory;
	m_actionFactory = nullptr;
	delete m_entityFactory;
	m_entityFactory = nullptr;
	delete m_inputParser;
	m_inputParser = nullptr;
	delete m_inputReader;
	m_inputReader = nullptr;
}


LoopStatus GameManager::init()
{
	m_player = m_gameDataLoader->loadGameData(CONFIG_FILE_PATH);
	if (m_player)
	{
		m_player->setActionFactory(m_actionFactory);
		// After all the initialization, log the welcome message
		consoleLog(m_gameDataLoader->getWelcomeMessage());
		m_loopStatus = LoopStatus::CONTINUE;
	}
	else
	{
		consoleLog("An error occured while loading the game configuration file!\n(Press Enter to close the window)");
		m_loopStatus = LoopStatus::INIT_ERROR;
	}
	return LoopStatus::CONTINUE;
}


LoopStatus GameManager::update()
{
	// Adquire input and pass to World
	if (m_loopStatus == LoopStatus::CONTINUE)
	{
		processInput(m_inputReader->getInput());
		if (m_loopStatus == LoopStatus::EXIT)
		{
			consoleLog(m_gameDataLoader->getExitMessage() + "\n(Press Enter to close the window)");
		}
	}
	else
	{
		if (m_inputReader->getEnter())
		{
			return m_loopStatus;
		}
	}
	return LoopStatus::CONTINUE;
}


LoopStatus GameManager::close()
{
	m_actionFactory->close();
	m_entityFactory->close();
	return LoopStatus::EXIT;
}


LoopStatus GameManager::processInput(const std::string& userInput)
{
	if (userInput != "")
	{
		Instruction instruction = m_inputParser->parse(userInput);
		switch (instruction.instructionType)
		{
		case InstructionType::ERROR:
			consoleLog(instruction.param1);
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
			m_player->executeInstruction(instruction);
			break;
		}
	}
	// Check whether a GameEnd request has been made
	if (m_loopStatus == LoopStatus::CONTINUE && isGameEndRequested)
	{
		m_loopStatus = LoopStatus::EXIT;
	}
	return m_loopStatus;
}

void GameManager::logHelpMessage() const
{
	std::string message = "INFORMATION:\n";
	message += "The commands that you can perform in the game and their effects are the following:\n\n";
	message += "  HELP: Display this list of actions.\n";
	message += "  QUIT: Quit the game.\n";
	message += "  LOOK: Display a description of the room you are currently in and the items therein.\n";
	message += "  INVENTORY: Display a description of the items you are currently holding.\n";
	message += "  GO direction: Attempt to exit the current room towards the direction to enter another room.\n";
	message += "                Valid directions are NORTH, SOUTH, EAST and WEST.\n";
	message += "  TAKE item: Attempt to pick up an item and put it in your inventory.\n";
	message += "  DROP item: Attempt to drop an item from your inventory.\n";
	message += "  INSPECT item: Display a detailed description of the item.\n";
	message += "  OPEN item: Attempt to open an item.\n";
	message += "  USE item1 ON item2: Attempt to use the item1 on the item2.\n";
	message += "  PUT item1 IN item2: Attempt to place item1 within item2.\n";
	message += "                      Both items must be in your inventory for this action to succeed.\n";
	message += "\nDo note that commands and item names are not case-sensitive,\nso you can freely type everything in lowercase if you want.";
	consoleLog(message);
}
