#include "Player.h"

#include <assert.h>
#include "Action.h"
#include "ActionType.h"
#include "Direction.h"
#include "EntityType.h" 
#include "Exit.h"
#include "Instruction.h"
#include "InstructionType.h"
#include "Item.h"
#include "Room.h"
#include "globals.h"


Player::Player(int id, const std::string& name, const std::string& description, int maxItems, Room* startingRoom)
	: Entity(id, EntityType::PLAYER, name, description, true), m_maxItems(maxItems), m_location(startingRoom)
{
	assert(m_maxItems > 0 && m_location);
}


Player::~Player()
{
}


void Player::setActionFactory(ActionFactory* actionFactory)
{
	assert(actionFactory);
	m_actionFactory = actionFactory;
}


void Player::executeInstruction(const Instruction& instruction)
{
	switch (instruction.instructionType)
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
		OutputLog("ERROR: Missing implementation for player instruction of type %s.", getStringFromInstruction(instruction.instructionType));
		assert(false);
		break;
	}
}


bool Player::canAddChild(const Entity* child) const
{
	assert(child);
	return child->getType() == EntityType::ITEM;
}


void Player::addChild(Entity* child)
{
	assert(canAddChild(child));
	Entity::addChild(child);
	updateLightStatus();
}


void Player::removeChild(const Entity* entity)
{
	assert(entity);
	Entity::removeChild(entity);
	updateLightStatus();
}


void Player::look() const
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


void Player::inventory() const
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


bool Player::go(const Instruction& instruction)
{
	bool success;
	// Ask room if there's an exit in that direction -> Room::getExit

	Direction direction = getDirectionFromString(instruction.param1);
	if (direction == Direction::_UNDEFINED)
	{
		consoleLog(instruction.param1 + " doesn't seem to be a valid direction to go to.");
		success = false;
	}
	else if (Exit* exit = m_location->getExit(direction))
	{
		if (exit->isLocked())
		{
			consoleLog(exit->getLockedDescription());
			success = false;
		}
		else
		{
			// So there is an exit and it's unlocked...
			// Move to that room
			Room* previousRoom = m_location;
			m_location = exit->getTargetRoom();
			consoleLog("You are now in the " + m_location->getName() + ".");

			// Perform actions if available
			if (Action* goAction = m_actionFactory->getAction(ActionType::GO, previousRoom, m_location))
			{
				// previousRoom to newRoom motion
				goAction->performAction();
			}
			if (Action* goAction = m_actionFactory->getAction(ActionType::GO, previousRoom, nullptr))
			{
				// previousRoom exit
				goAction->performAction();
			}
			if (Action* goAction = m_actionFactory->getAction(ActionType::GO, nullptr, m_location))
			{
				// newRoom entry
				goAction->performAction();
			}
			success = true;
		}
	}
	else
	{
		consoleLog("You can't go " + getStringFromDirection(direction) + " from here.");
		success = false;
	}

	return success;
}


bool Player::take(const Instruction& instruction)
{
	bool success;
	// Must check that type == Item && there’s room in children (inheritted from Entity)

	Entity* target = nullptr;
	// Try to get an Entity with the requested name from the room.	
	if (target = m_hasLight ? m_location->getChild(instruction.param1) : m_location->getChildInDarkness(instruction.param1))
	{
		// Only items can be taken
		if (target->getType() != EntityType::ITEM)
		{
			consoleLog("You can't take the " + target->getName() + ".");
			success = false;
		}
		else if (static_cast<int>(m_children.size()) < m_maxItems)
		{
			target->setParent(this);
			consoleLog("You have taken the " + target->getName() + ".");
			
			// Perform action if available
			if (Action* takeAction = m_actionFactory->getAction(ActionType::TAKE, target))
			{
				takeAction->performAction();
			}
			success = true;
		}
		else
		{
			consoleLog("You can't carry any more items. You only have two very tiny hands.\nDrop something if you wish to take the " + target->getName() + ".");
			success = false;
		}
	}
	// Try to get an Entity within the children of the room's children to customize the message.
	else if (target = m_hasLight ? m_location->getChild(instruction.param1, true) : m_location->getChildInDarkness(instruction.param1, true))
	{
		consoleLog("You can't take the " + target->getName() + " on its own, because it is within the " + target->getParent()->getName() + ".");
		success = false;
	}
	else
	{
		consoleLog("There doesn't seem to be anything called " + instruction.param1 + " in here.");
		success = false;
	}

	return success;
}


bool Player::drop(const Instruction& instruction)
{
	bool success;
	// Must check that target is in children

	Entity* target = nullptr;
	// Try to get an Entity with the requested name from the player's inventory (it will be an ITEM).
	if (target = getChild(instruction.param1))
	{
		target->setParent(m_location);
		consoleLog("You have dropped the " + target->getName() + ".");
		if (Action* dropAction = m_actionFactory->getAction(ActionType::DROP, target))
		{
			dropAction->performAction();
		}
		success = true;
	}
	// Try to get an Entity within the children of the player's inventory to customize the message.
	else if (target = getChild(instruction.param1, true))
	{
		consoleLog("You can't drop the " + target->getName() + " on its own, because it is within the " + target->getParent()->getName() + ".");
		success = false;
	}
	else
	{
		consoleLog("You don't have the " + instruction.param1 + " that you intend to drop.");
		success = false;
	}

	return success;
}


bool Player::inspect(const Instruction& instruction) const
{
	bool success;
	// Item or interactable
	
	Entity* target;
	// Try to get an Entity* with the requested name from the palyer's inventory or the room.
	target = getChild(instruction.param1, true);
	if (!target)
	{
		target = m_hasLight ? m_location->getChild(instruction.param1, true) : m_location->getChildInDarkness(instruction.param1, true);
	}

	if (target)
	{
		// Check if the target is an item or interactable
		if (target->getType() != EntityType::ITEM && target->getType() != EntityType::INTERACTABLE)
		{
			consoleLog("You can't inspect the " + target->getName() + ".");
			success = false;
		}
		else
		{
			consoleLog(target->getDetailedDescription());
			success = true;
		}
	}
	else
	{
		consoleLog("There doesn't seem to be anything called " + instruction.param1 + " in here.");
		success = false;
	}

	return success;
}


bool Player::open(const Instruction& instruction) const
{
	bool success;
	// Must check type & presence in room

	// Try to get an Entity* with the requested name form the Room.
	Entity* target = m_hasLight ? m_location->getChild(instruction.param1) : m_location->getChildInDarkness(instruction.param1);
	if (target)
	{
		// Try to get an InteractableOpen* with the requested Entity name from the World.
		assert(m_actionFactory);
		Action* interactableOpen = m_actionFactory->getAction(ActionType::INTERACTABLE_OPEN, target);
		if (interactableOpen)
		{
			interactableOpen->performAction();
			success = true;
		}
		else
		{
			consoleLog("You can't open the " + target->getName() + ".");
			success = false;
		}
	}
	else
	{
		consoleLog("There doesn't seem to be anything called " + instruction.param1 + " in here.");
		success = false;
	}

	return success;
}


bool Player::use(const Instruction& instruction)
{
	bool success;
	// Must verify that item can be used in target (done by item internally)

	// Try to get an Entity* with the requested name from the Inventory (will be an Item).
	Entity* item = getChild(instruction.param1, true);
	if (item)
	{
		// Try to get an Entity* with the requested name form the Room.
		Entity* target = nullptr;
		if (target = m_hasLight ? m_location->getChild(instruction.param2) : m_location->getChildInDarkness(instruction.param2))
		{
			success = false;
			if (target->getType() == EntityType::INTERACTABLE)
			{
				assert(m_actionFactory);
				Action* itemUse = m_actionFactory->getAction(ActionType::ITEM_USE, item, target);
				if (itemUse)
				{
					itemUse->performAction();
					success = true;
				}
			}
			
			if (!success)
			{
				consoleLog("You can't use the " + item->getName() + " on the " + target->getName() + ".");
			}
		}
		else if (target = getChild(instruction.param2, true))
		{
			consoleLog("You can't use the " + item->getName() + " on the " + target->getName() + ".");
			success = false;
		}
		else
		{
			consoleLog("There doesn't seem to be anything called " + instruction.param2 + " in here.");
			success = false;
		}
	}
	else
	{
		consoleLog("You don't have the " + instruction.param1 + " that you intend to use.");
		success = false;
	}

	return success;
}


bool Player::put(const Instruction& instruction)
{
	bool success;
	// Place an item inside another item (if allowed)

	// Try to get an Entity* with the requested name from the Inventory (will be an Item).
	Entity* item = getChild(instruction.param1);
	if (item)
	{
		// Try to get a second Entity* with the requested name form the Inventory (will also be an item).
		if (Entity* containerItem = getChild(instruction.param2))
		{
			assert(m_actionFactory);
			Action* itemPut = m_actionFactory->getAction(ActionType::ITEM_PUT, item, containerItem);
			if (itemPut)
			{
				itemPut->performAction();
				success = true;
			}
			else
			{
				consoleLog("You can't put the " + item->getName() + " in the " + containerItem->getName() + ".");
				success = false;
			}
		}
		else
		{
			consoleLog("There is no " + instruction.param2 + " in your inventory to put the " + item->getName() + " into.");
			success = false;
		}
	}
	else
	{
		consoleLog("You don't have the " + instruction.param1 + " that you intend to put into something else.");
		success = false;
	}

	return success;
}


void Player::updateLightStatus()
{
	m_hasLight = false;

	for (Entity* item : m_children)
	{
		if ((static_cast<Item*>(item))->hasLight())
		{
			m_hasLight = true;
			break;
		}
	}
}
