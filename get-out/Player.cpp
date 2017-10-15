#include "Player.h"
#include <assert.h>
#include "globals.h"
#include "EntityType.h" 
#include "Instruction.h"
#include "ActionType.h"
#include "Room.h"
#include "Direction.h"
#include "Exit.h"


Player::Player(EntityType type, std::string name, std::string description, unsigned int maxItems, Room* startingRoom)
	: Entity(type, name, description), m_maxItems(maxItems), m_location(startingRoom)
{
	assert(m_location);
}


Player::~Player()
{
}


void Player::executeInstruction(const Instruction * instruction)
{
	switch (instruction->actionType)
	{
	case ActionType::LOOK:
		look();
		break;
	case ActionType::GO:
		go(instruction->direction);
		break;
		/*
	case ActionType::TAKE:
		take(instruction->entity);
		break;
	case ActionType::DROP:
		drop(instruction->entity);
		break;
	case ActionType::INSPECT:
		inspect(instruction->entity);
		break;
	case ActionType::OPEN:
		open(instruction->interactableOpen);
		break;
	case ActionType::USE:
		use(instruction->itemUse);
		break;
	case ActionType::PUT:
		put(instruction->itemPut);
		break;
	*/
	default:
		consoleLog("Missing implementation for player instruction of type " + getStringFromAction(instruction->actionType));
		break;
	}
	
}

bool Player::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}

void Player::look()
{
	consoleLog(m_location->getDescription());
}

bool Player::go(Direction direction)
{
	bool success = false;
	Exit* exit = m_location->getExit(direction);
	if (exit)
	{
		if (exit->isLocked())
		{
			consoleLog(exit->getLockedDescription());
			return false;
		}
		else
		{
			// So there is an exit and it's unlocked...
			m_location = exit->getTargetRoom();
			consoleLog("You are now in the " + m_location->getName() + ".");
			return true;
		}
	}
	else
	{
		consoleLog("There's nowhere to go in the " + getStringFromDirection(direction) + ".");
		return false;
	}
}
