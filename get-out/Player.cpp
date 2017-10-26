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
#include "Item.h"

#include "World.h"


Player::Player(int id, const std::string& name, const std::string& description, int maxItems, Room* startingRoom)
	: Entity(id, EntityType::PLAYER, name, description, true), m_maxItems(maxItems), m_location(startingRoom)
{
	assert(m_location && m_maxItems > 0);
}


Player::~Player()
{
}


void Player::executeInstruction(const Instruction * instruction)
{
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
	case InstructionType::PUT:
		put(instruction);
		break;
	default:
		OutputLog("ERROR: Missing implementation for player instruction of type %s.", getStringFromInstruction(instruction->instructionType));
		assert(false);
		break;
	}
	
}


bool Player::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}


void Player::look()
{
	if (m_hasLight)
	{
		consoleLog(m_location->getDescription());
	}
	else
	{
		consoleLog(m_location->getDescriptionInDarkness());
	}
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
	Entity* target = nullptr;
	// Try to get an Entity with the requested name from the room.
	target = m_hasLight ? m_location->getChild(instruction->param1) : m_location->getChildInDarkness(instruction->param1);
	
	if (target)
	{
		// Only items can be taken
		if (target->getType() != EntityType::ITEM)
		{
			consoleLog("You can't take the " + target->getName() + ".");
			return false;
		}
		if (static_cast<int>(m_children.size()) < m_maxItems)
		{
			target->setParent(this);
			if (static_cast<Item*>(target)->hasLight())
			{
				m_hasLight = true;
			}
			consoleLog("You have taken the " + target->getName() + ".");
			return true;
		}
		consoleLog("You can't carry any more items. You only have two very tiny hands.\nDrop something if you wish to take the " + target->getName() + ".");
		return false;
	}
	// Try to get an Entity within the children of the room's children to customize the message.
	target = m_hasLight ? m_location->getChild(instruction->param1, true) : m_location->getChildInDarkness(instruction->param1, true);

	if (target)
	{
		consoleLog("You can't take the " + target->getName() + " on its own, because it is within the " + target->getParent()->getName() + ".");
		return false;
	}
	consoleLog("There is nothing called " + instruction->param1 + " that you can see here.");
	return false;
}


bool Player::drop(const Instruction* instruction)
{
	Entity* target = nullptr;
	// Try to get an Entity with the requested name from the player's inventory (it will be an ITEM).
	target = getChild(instruction->param1);
	if (target)
	{
		target->setParent(m_location);
		updateLightStatus();
		consoleLog("You have dropped the " + target->getName() + ".");
		return true;
	}
	// Try to get an Entity within the children of the player's inventory to customize the message.
	target = getChild(instruction->param1, true);
	if (target)
	{
		consoleLog("You can't drop the " + target->getName() + " on its own, because it is within the " + target->getParent()->getName() + ".");
		return false;
	}
	consoleLog("You don't have the " + instruction->param1 + " that you intend to drop.");
	return false;
}


bool Player::inspect(const Instruction* instruction)
{
	Entity* target;
	// Try to get an Entity* with the requested name from the palyer's inventory or the room.
	target = getChild(instruction->param1, true);
	if (!target)
	{
		target = m_hasLight ? m_location->getChild(instruction->param1, true) : m_location->getChildInDarkness(instruction->param1, true);
	}
	if (target)
	{
		// Check if the target is an item or interactable
		if (target->getType() != EntityType::ITEM && target->getType() != EntityType::INTERACTABLE)
		{
			consoleLog("You can't inspect the " + target->getName() + ".");
			return false;
		}
		consoleLog(target->getDetailedDescription());
		return true;
	}
	consoleLog("There is no " + instruction->param1 + " to inspect that you can see here.");
	return false;
}


bool Player::open(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name form the Room.
	Entity* target = m_hasLight ? m_location->getChild(instruction->param1) : m_location->getChildInDarkness(instruction->param1);
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
	consoleLog("The is no " + instruction->param1 + " to open that you can see here.");
	return false;
}


bool Player::use(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name from the Inventory (will be an Item).
	Entity* item = getChild(instruction->param1, true);
	if (item)
	{
		// Try to get an Entity* with the requested name form the Room.
		Entity* interactable = m_hasLight ? m_location->getChild(instruction->param2) : m_location->getChildInDarkness(instruction->param2);
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
		consoleLog("There is no " + instruction->param2 + " that you can see here to use your " + item->getName() + " on.");
		return false;
	}
	consoleLog("You don't have the " + instruction->param1 + " that you intend to use.");
	return false;
}


bool Player::put(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name from the Inventory (will be an Item).
	Entity* item = getChild(instruction->param1);
	if (item)
	{
		// Try to get a second Entity* with the requested name form the Inventory (will also be an item).
		Entity* containerItem = getChild(instruction->param2);
		if (containerItem)
		{
			Action* itemPut = world->getAction(ActionType::ItemPut, item->getName(), containerItem->getName());
			if (itemPut)
			{
				itemPut->performAction();
				return true;
			}
			consoleLog("You can't put the " + item->getName() + " in the " + containerItem->getName() + ".");
			return false;
		}
		consoleLog("There is no " + instruction->param2 + " in your inventory to put the " + item->getName() + " into.");
		return false;
	}
	consoleLog("You don't have the " + instruction->param1 + " that you intend to put into something else.");
	return false;
}


void Player::updateLightStatus()
{
	for (Entity* item : m_children)
	{
		if ((static_cast<Item*>(item))->hasLight())
		{
			m_hasLight = true;
			return;
		}
	}
	m_hasLight = false;
}
