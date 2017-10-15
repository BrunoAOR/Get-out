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
	case ActionType::INVENTORY:
		inventory();
		break;
	case ActionType::GO:
		go(instruction->direction);
		break;
	case ActionType::TAKE:
		take(instruction->entity);
		break;
	case ActionType::DROP:
		drop(instruction->entity);
		break;
	/*
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

void Player::inventory()
{
	if (m_children.size() == 0)
	{
		consoleLog("You are currently not holding any items.");
	}
	else
	{
		std::string message = "You have the following items in hand:";
		for (Entity* item : m_children)
		{
			message += "\n  " + item->getDescription();
		}
		consoleLog(message);
	}
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

bool Player::take(Entity * target)
{
	// Only items can be taken
	if (target->getType() != EntityType::ITEM)
	{
		consoleLog("You can't take that " + target->getName() + ".");
		return false;
	}
	// Check if item is in our current room
	if (m_location->hasItem(target))
	{
		if (m_children.size() < m_maxItems)
		{
			target->setParent(this);
			consoleLog("You have taken the " + target->getName() + ".");
			return true;
		}
		else
		{
			consoleLog("You can't carry any more items. You only have two very tiny hands.\nDrop something if you wish to take the " + target->getName() + ".");
			return false;
		}
	}
	else
	{
		consoleLog("There is no such thing as " + target->getName() + " in here.");
		return false;
	}
}

bool Player::drop(Entity * target)
{
	// Check if the player is holding said Entity (should be an item)
	for (Entity* child : m_children)
	{
		if (child == target)
		{
			target->setParent(m_location);
			consoleLog("You have dropped the " + target->getName() + ".");
			return true;
		}
	}
	consoleLog("You don't have the " + target->getName() + " that you intend to drop.");
	return false;
}
