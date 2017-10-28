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


Player::Player(int id, const std::string& name, const std::string& description, int maxItems, Room* startingRoom)
	: Entity(id, EntityType::PLAYER, name, description, true), m_maxItems(maxItems), m_location(startingRoom)
{
	assert(m_maxItems > 0 && m_location);
}


Player::~Player()
{
}


void Player::setActionFactory(ActionFactory * actionFactory)
{
	m_actionFactory = actionFactory;
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


bool Player::canAddChild(const Entity * child) const
{
	return child->getType() == EntityType::ITEM;
}


void Player::addChild(Entity * child)
{
	updateLightStatus();
	Entity::addChild(child);
}


void Player::removeChild(const Entity * entity)
{
	updateLightStatus();
	Entity::removeChild(entity);
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
		consoleLog(instruction->param1 + " doesn't seem to be a valid direction to go to.");
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
		consoleLog("You can't go " + getStringFromDirection(direction) + " from here.");
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
	consoleLog("There doesn't seem to be anything called " + instruction->param1 + " in here.");
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
	consoleLog("There doesn't seem to be anything called " + instruction->param1 + " in here.");
	return false;
}


bool Player::open(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name form the Room.
	Entity* target = m_hasLight ? m_location->getChild(instruction->param1) : m_location->getChildInDarkness(instruction->param1);
	if (target)
	{
		// Try to get an InteractableOpen* with the requested Entity name from the World.
		assert(m_actionFactory);
		Action* interactableOpen = m_actionFactory->getAction(ActionType::InteractableOpen, target);
		if (interactableOpen)
		{
			interactableOpen->performAction();
			return true;
		}
		consoleLog("You can't open the " + target->getName() + ".");
		return false;
	}
	consoleLog("There doesn't seem to be anything called " + instruction->param1 + " in here.");
	return false;
}


bool Player::use(const Instruction * instruction)
{
	// Try to get an Entity* with the requested name from the Inventory (will be an Item).
	Entity* item = getChild(instruction->param1, true);
	if (item)
	{
		// Try to get an Entity* with the requested name form the Room.
		Entity* target = m_hasLight ? m_location->getChild(instruction->param2) : m_location->getChildInDarkness(instruction->param2);
		if (target)
		{
			if (target->getType() == EntityType::INTERACTABLE)
			{
				assert(m_actionFactory);
				Action* itemUse = m_actionFactory->getAction(ActionType::ItemUse, item, target);
				if (itemUse)
				{
					itemUse->performAction();
					return true;
				}
			}
			consoleLog("You can't use the " + item->getName() + " on the " + target->getName() + ".");
			return false;
		}
		if (!target)
		{
			target = getChild(instruction->param2, true);
			if (target)
			{
				consoleLog("You can't use the " + item->getName() + " on the " + target->getName() + ".");
				return false;
			}
		}
		consoleLog("There doesn't seem to be anything called " + instruction->param2 + " in here.");
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
			assert(m_actionFactory);
			Action* itemPut = m_actionFactory->getAction(ActionType::ItemPut, item, containerItem);
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
