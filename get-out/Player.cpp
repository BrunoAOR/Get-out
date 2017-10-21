#include "Player.h"
#include <assert.h>
#include "globals.h"
#include "EntityType.h" 
#include "Instruction.h"
#include "InstructionType.h"
#include "Room.h"
#include "Direction.h"
#include "Exit.h"
#include "Action.h"
#include "ActionType.h"

#include "World.h"
#include "InteractableOpen.h"


Player::Player(std::string name, std::string m_description, unsigned int maxItems, Room* startingRoom)
	: Entity(EntityType::PLAYER, name, m_description), m_maxItems(maxItems), m_location(startingRoom)
{
	assert(m_location);
}


Player::~Player()
{
}


void Player::executeInstruction(const Instruction * instruction)
{
	// TODO: Implement the darkness behaviour
	switch (instruction->instructionType)
	{
	case InstructionType::LOOK:
		look();
		break;
	case InstructionType::INVENTORY:
		inventory();
		break;
	case InstructionType::GO:
		go(instruction);
		break;
	case InstructionType::TAKE:
		take(instruction);
		break;
	case InstructionType::DROP:
		drop(instruction);
		break;
	case InstructionType::INSPECT:
		inspect(instruction);
		break;
	case InstructionType::OPEN:
		open(instruction);
		break;
	case InstructionType::USE:
		use(instruction);
		break;
	/*
	case InstructionType::PUT:
		put(instruction);
		break;
	*/
	default:
		consoleLog("Missing implementation for player instruction of type " + getStringFromInstruction(instruction->instructionType));
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


bool Player::go(const Instruction* instruction)
{
	Direction direction = getDirectionFromString(instruction->param1);
	if (direction == Direction::_UNDEFINED)
	{
		consoleLog(instruction->param1 + " doesn't seem to be a valid direction to GO to.");
		return false;
	}

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


bool Player::take(const Instruction* instruction)
{
	// Try to get an Entity with the requested name from the room.
	Entity* target = m_location->getChild(instruction->param1);
	if (target)
	{
		// Only items can be taken
		if (target->getType() != EntityType::ITEM)
		{
			consoleLog("You can't take the " + target->getName() + ".");
			return false;
		}
		if (m_children.size() < m_maxItems)
		{
			target->setParent(this);
			consoleLog("You have taken the " + target->getName() + ".");
			return true;
		}
		consoleLog("You can't carry any more items. You only have two very tiny hands.\nDrop something if you wish to take the " + target->getName() + ".");
		return false;
	}
	else
	{
		consoleLog("There is nothing called " + instruction->param1 + " in here.");
		return false;
	}

}


bool Player::drop(const Instruction* instruction)
{
	// Try to get an Entity with the requested name from the player's inventory (it will be an ITEM).
	Entity* target = getChild(instruction->param1);
	if (target)
	{
		target->setParent(m_location);
		consoleLog("You have dropped the " + target->getName() + ".");
		return true;
	}
	consoleLog("You don't have the " + instruction->param1 + " that you intend to drop.");
	return false;
}


bool Player::inspect(const Instruction* instruction)
{
	// Try to get an Entity* with the requested name from the palyer's inventory or the room.
	Entity* target = getChild(instruction->param1);
	if (!target)
	{
		target = m_location->getChild(instruction->param1);
	}
	if (target)
	{
		// Check if the target is an item or interactable
		if (target->getType() != EntityType::ITEM && target->getType() != EntityType::INTERACTABLE)
		{
			consoleLog("You can't inspect the " + target->getName() + ".");
			return false;
		}

		std::string detailedDescription = target->getDetailedDescription();
		if (detailedDescription == "")
		{
			consoleLog("There is nothing special about the " + target->getName());
		}
		else
		{
			consoleLog(detailedDescription);
		}
		return true;
	}
	consoleLog("There is no " + instruction->param1 + " to inspect here.");
	return false;
}


bool Player::open(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name form the Room.
	Entity* target = m_location->getChild(instruction->param1);
	if (target)
	{
		// Try to get an InteractableOpen* with the requested Entity name from the World.
		Action* interactableOpen = world->getAction(ActionType::InteractableOpen, instruction->param1);
		if (interactableOpen)
		{
			interactableOpen->performAction();
			return true;
		}
		consoleLog("You can't open the " + target->getName() + ".");
		return false;
	}
	consoleLog("The is no " + instruction->param1 + " to open here.");
	return false;
}


bool Player::use(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name from the Inventory (will be an Item).
	Entity* item = getChild(instruction->param1);
	if (item)
	{
		// Try to get an Entity* with the requested name form the Room.
		Entity* interactable = m_location->getChild(instruction->param2);
		if (interactable)
		{
			if (interactable->getType() == EntityType::INTERACTABLE)
			{
				Action* itemUse = world->getAction(ActionType::ItemUse, item->getName(), interactable->getName());
				if (itemUse)
				{
					itemUse->performAction();
					return true;
				}
			}
			consoleLog("You can't use the " + item->getName() + " on the " + interactable->getName() + ".");
			return false;
		}
		consoleLog("There is no " + instruction->param2 + " in the room to use your " + item->getName() + " on.");
		return false;
	}
	consoleLog("You don't have the " + instruction->param1 + " that you intend to use.");
	return false;
}
