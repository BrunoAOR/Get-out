#include "GameManager.h"

#include "ActionFactory.h"
#include "EntityFactory.h"
#include "GameData.h"
#include "GameDataLoader.h"
#include "InputParser.h"
#include "InputReader.h"
#include "Instruction.h"
#include "InstructionType.h"
#include "Player.h"


GameManager::GameManager()
{
	mInputReader = new InputReader();
	mInputParser = new InputParser();
	mEntityFactory = new EntityFactory();
	mActionFactory = new ActionFactory(mEntityFactory);
	mGameDataLoader = new GameDataLoader(mEntityFactory, mActionFactory);
}


GameManager::~GameManager()
{
	delete mGameDataLoader;
	mGameDataLoader = nullptr; 
	delete mActionFactory;
	mActionFactory = nullptr;
	delete mEntityFactory;
	mEntityFactory = nullptr;
	delete mInputParser;
	mInputParser = nullptr;
	delete mInputReader;
	mInputReader = nullptr;
}


LoopStatus GameManager::init()
{
	mPlayer = mGameDataLoader->loadGameData(GameData::LoadFromJson(CONFIG_FILE_PATH));
	if (mPlayer)
	{
		mPlayer->setActionFactory(mActionFactory);
		// After all the initialization, log the welcome lMessage
		consoleLog(mGameDataLoader->getWelcomeMessage());
		mLoopStatus = LoopStatus::CONTINUE;
	}
	else
	{
		consoleLog("An error occured while loading the game configuration file!\n(Press Enter to close the window)");
		mLoopStatus = LoopStatus::INIT_ERROR;
	}
	return LoopStatus::CONTINUE;
}


LoopStatus GameManager::update()
{
	// Adquire input and pass to World
	if (mLoopStatus == LoopStatus::CONTINUE)
	{
		processInput(mInputReader->getInput());
		if (mLoopStatus == LoopStatus::EXIT)
		{
			consoleLog(mGameDataLoader->getExitMessage() + "\n(Press Enter to close the window)");
		}
	}
	else
	{
		if (mInputReader->getEnter())
		{
			return mLoopStatus;
		}
	}
	return LoopStatus::CONTINUE;
}


LoopStatus GameManager::close()
{
	mActionFactory->close();
	mEntityFactory->close();
	return LoopStatus::EXIT;
}


LoopStatus GameManager::processInput(const std::string& aUserInput)
{
	if (aUserInput != "")
	{
		Instruction lInstruction = mInputParser->parse(aUserInput);
		switch (lInstruction.mInstructionType)
		{
		case InstructionType::ERROR:
			consoleLog(lInstruction.mParam1);
			break;
		case InstructionType::QUIT:
			mLoopStatus = LoopStatus::EXIT;
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
			mPlayer->executeInstruction(lInstruction);
			break;
		}
	}
	// Check whether a GameEnd request has been made
	if (mLoopStatus == LoopStatus::CONTINUE && gIsGameEndRequested)
	{
		mLoopStatus = LoopStatus::EXIT;
	}
	return mLoopStatus;
}

void GameManager::logHelpMessage() const
{
	std::string lMessage = "INFORMATION:\n";
	lMessage += "The commands that you can perform in the game and their effects are the following:\n\n";
	lMessage += "  HELP: Display this list of actions.\n";
	lMessage += "  QUIT: Quit the game.\n";
	lMessage += "  LOOK: Display a description of the room you are currently in and the items therein.\n";
	lMessage += "  INVENTORY: Display a description of the items you are currently holding.\n";
	lMessage += "  GO direction: Attempt to exit the current room towards the direction to enter another room.\n";
	lMessage += "                Valid directions are NORTH, SOUTH, EAST and WEST.\n";
	lMessage += "  TAKE item: Attempt to pick up an item and put it in your inventory.\n";
	lMessage += "  DROP item: Attempt to drop an item from your inventory.\n";
	lMessage += "  INSPECT item: Display a detailed description of the item.\n";
	lMessage += "  OPEN item: Attempt to open an item.\n";
	lMessage += "  USE item1 ON item2: Attempt to use the item1 on the item2.\n";
	lMessage += "  PUT item1 IN item2: Attempt to place item1 within item2.\n";
	lMessage += "                      Both items must be in your inventory for this action to succeed.\n";
	lMessage += "\nDo note that commands and item names are not case-sensitive,\nso you can freely type everything in lowercase if you want.";
	consoleLog(lMessage);
}
