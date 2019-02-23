#include "Player.h"

#include "globals.h"
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


Player::Player(int aId, const std::string& aName, const std::string& aDescription, int aMaxItems, Room* aStartingRoom)
	: Entity(aId, EntityType::PLAYER, aName, aDescription, true), mMaxItems(aMaxItems), mLocation(aStartingRoom)
{
	ASSERT(mMaxItems > 0 && mLocation);
}


Player::~Player()
{
}


void Player::setActionFactory(ActionFactory* aActionFactory)
{
	ASSERT(aActionFactory);
	mActionFactory = aActionFactory;
}


void Player::executeInstruction(const Instruction& aInstruction)
{
	switch (aInstruction.mInstructionType)
	{
	case InstructionType::LOOK:
		look();
		break;
	case InstructionType::INVENTORY:
		inventory();
		break;
	case InstructionType::GO:
		go(aInstruction);
		break;
	case InstructionType::TAKE:
		take(aInstruction);
		break;
	case InstructionType::DROP:
		drop(aInstruction);
		break;
	case InstructionType::INSPECT:
		inspect(aInstruction);
		break;
	case InstructionType::OPEN:
		open(aInstruction);
		break;
	case InstructionType::USE:
		use(aInstruction);
		break;
	case InstructionType::PUT:
		put(aInstruction);
		break;
	default:
		OUTPUT_LOG("ERROR: Missing implementation for player instruction of type %s.", getStringFromInstruction(aInstruction.mInstructionType));
		ASSERT(false);
		break;
	}
}


bool Player::canAddChild(const Entity* aChild) const
{
	ASSERT(aChild);
	return aChild->getType() == EntityType::ITEM;
}


void Player::addChild(Entity* aChild)
{
	ASSERT(canAddChild(aChild));
	Entity::addChild(aChild);
	updateLightStatus();
}


void Player::removeChild(const Entity* aEntity)
{
	ASSERT(aEntity);
	Entity::removeChild(aEntity);
	updateLightStatus();
}


void Player::look() const
{
	if (mHasLight)
	{
		consoleLog(mLocation->getDescription());
	}
	else
	{
		consoleLog(mLocation->getDescriptionInDarkness());
	}
}


void Player::inventory() const
{
	if (mChildren.size() == 0)
	{
		consoleLog("You are currently not holding any items.");
	}
	else
	{
		std::string lMessage = "You have the following items in hand:";
		for (Entity* lItem : mChildren)
		{
			lMessage += "\n  " + lItem->getDescription();
		}
		consoleLog(lMessage);
	}
}


bool Player::go(const Instruction& aInstruction)
{
	bool lSuccess;
	// Ask room if there's an lExit in that lDirection -> Room::getExit

	Direction lDirection = getDirectionFromString(aInstruction.mParam1);
	if (lDirection == Direction::_UNDEFINED)
	{
		consoleLog(aInstruction.mParam1 + " doesn't seem to be a valid direction to go to.");
		lSuccess = false;
	}
	else if (Exit* lExit = mLocation->getExit(lDirection))
	{
		if (lExit->isLocked())
		{
			consoleLog(lExit->getLockedDescription());
			lSuccess = false;
		}
		else
		{
			// So there is an lExit and it's unlocked...
			// Move to that room
			Room* lPreviousRoom = mLocation;
			mLocation = lExit->getTargetRoom();
			consoleLog("You are now in the " + mLocation->getName() + ".");

			// Perform actions if available
			if (Action* lGoAction = mActionFactory->getAction(ActionType::GO, lPreviousRoom, mLocation))
			{
				// lPreviousRoom to newRoom motion
				lGoAction->performAction();
			}
			if (Action* lGoAction = mActionFactory->getAction(ActionType::GO, lPreviousRoom, nullptr))
			{
				// lPreviousRoom lExit
				lGoAction->performAction();
			}
			if (Action* lGoAction = mActionFactory->getAction(ActionType::GO, nullptr, mLocation))
			{
				// newRoom entry
				lGoAction->performAction();
			}
			lSuccess = true;
		}
	}
	else
	{
		consoleLog("You can't go " + getStringFromDirection(lDirection) + " from here.");
		lSuccess = false;
	}

	return lSuccess;
}


bool Player::take(const Instruction& aInstruction)
{
	bool lSuccess;
	// Must check that type == Item && there’s room in children (inheritted from Entity)

	Entity* lTarget = nullptr;
	// Try to get an Entity with the requested aName from the room.	
	if (lTarget = mHasLight ? mLocation->getChild(aInstruction.mParam1) : mLocation->getChildInDarkness(aInstruction.mParam1))
	{
		// Only items can be taken
		if (lTarget->getType() != EntityType::ITEM)
		{
			consoleLog("You can't take the " + lTarget->getName() + ".");
			lSuccess = false;
		}
		else if (static_cast<int>(mChildren.size()) < mMaxItems)
		{
			lTarget->setParent(this);
			consoleLog("You have taken the " + lTarget->getName() + ".");
			
			// Perform action if available
			if (Action* lTakeAction = mActionFactory->getAction(ActionType::TAKE, lTarget))
			{
				lTakeAction->performAction();
			}
			lSuccess = true;
		}
		else
		{
			consoleLog("You can't carry any more items. You only have two very tiny hands.\nDrop something if you wish to take the " + lTarget->getName() + ".");
			lSuccess = false;
		}
	}
	// Try to get an Entity within the children of the room's children to customize the lMessage.
	else if (lTarget = mHasLight ? mLocation->getChild(aInstruction.mParam1, true) : mLocation->getChildInDarkness(aInstruction.mParam1, true))
	{
		consoleLog("You can't take the " + lTarget->getName() + " on its own, because it is within the " + lTarget->getParent()->getName() + ".");
		lSuccess = false;
	}
	else
	{
		consoleLog("There doesn't seem to be anything called " + aInstruction.mParam1 + " in here.");
		lSuccess = false;
	}

	return lSuccess;
}


bool Player::drop(const Instruction& aInstruction)
{
	bool lSuccess;
	// Must check that lTarget is in children

	Entity* lTarget = nullptr;
	// Try to get an Entity with the requested aName from the player's inventory (it will be an ITEM).
	if (lTarget = getChild(aInstruction.mParam1))
	{
		lTarget->setParent(mLocation);
		consoleLog("You have dropped the " + lTarget->getName() + ".");
		if (Action* lDropAction = mActionFactory->getAction(ActionType::DROP, lTarget))
		{
			lDropAction->performAction();
		}
		lSuccess = true;
	}
	// Try to get an Entity within the children of the player's inventory to customize the lMessage.
	else if (lTarget = getChild(aInstruction.mParam1, true))
	{
		consoleLog("You can't drop the " + lTarget->getName() + " on its own, because it is within the " + lTarget->getParent()->getName() + ".");
		lSuccess = false;
	}
	else
	{
		consoleLog("You don't have the " + aInstruction.mParam1 + " that you intend to drop.");
		lSuccess = false;
	}

	return lSuccess;
}


bool Player::inspect(const Instruction& aInstruction) const
{
	bool lSuccess;
	// Item or interactable
	
	Entity* lTarget;
	// Try to get an Entity* with the requested aName from the palyer's inventory or the room.
	lTarget = getChild(aInstruction.mParam1, true);
	if (!lTarget)
	{
		lTarget = mHasLight ? mLocation->getChild(aInstruction.mParam1, true) : mLocation->getChildInDarkness(aInstruction.mParam1, true);
	}

	if (lTarget)
	{
		// Check if the lTarget is an lItem or interactable
		if (lTarget->getType() != EntityType::ITEM && lTarget->getType() != EntityType::INTERACTABLE)
		{
			consoleLog("You can't inspect the " + lTarget->getName() + ".");
			lSuccess = false;
		}
		else
		{
			consoleLog(lTarget->getDetailedDescription());
			lSuccess = true;
		}
	}
	else
	{
		consoleLog("There doesn't seem to be anything called " + aInstruction.mParam1 + " in here.");
		lSuccess = false;
	}

	return lSuccess;
}


bool Player::open(const Instruction& aInstruction) const
{
	bool lSuccess;
	// Must check type & presence in room

	// Try to get an Entity* with the requested aName form the Room.
	Entity* lTarget = mHasLight ? mLocation->getChild(aInstruction.mParam1) : mLocation->getChildInDarkness(aInstruction.mParam1);
	if (lTarget)
	{
		// Try to get an InteractableOpen* with the requested Entity aName from the World.
		ASSERT(mActionFactory);
		Action* lInteractableOpen = mActionFactory->getAction(ActionType::INTERACTABLE_OPEN, lTarget);
		if (lInteractableOpen)
		{
			lInteractableOpen->performAction();
			lSuccess = true;
		}
		else
		{
			consoleLog("You can't open the " + lTarget->getName() + ".");
			lSuccess = false;
		}
	}
	else
	{
		consoleLog("There doesn't seem to be anything called " + aInstruction.mParam1 + " in here.");
		lSuccess = false;
	}

	return lSuccess;
}


bool Player::use(const Instruction& aInstruction)
{
	bool lSuccess;
	// Must verify that lItem can be used in lTarget (done by lItem internally)

	// Try to get an Entity* with the requested aName from the Inventory (will be an Item).
	Entity* lItem = getChild(aInstruction.mParam1, true);
	if (lItem)
	{
		// Try to get an Entity* with the requested aName form the Room.
		Entity* lTarget = nullptr;
		if (lTarget = mHasLight ? mLocation->getChild(aInstruction.mParam2) : mLocation->getChildInDarkness(aInstruction.mParam2))
		{
			lSuccess = false;
			if (lTarget->getType() == EntityType::INTERACTABLE)
			{
				ASSERT(mActionFactory);
				Action* lItemUse = mActionFactory->getAction(ActionType::ITEM_USE, lItem, lTarget);
				if (lItemUse)
				{
					lItemUse->performAction();
					lSuccess = true;
				}
			}
			
			if (!lSuccess)
			{
				consoleLog("You can't use the " + lItem->getName() + " on the " + lTarget->getName() + ".");
			}
		}
		else if (lTarget = getChild(aInstruction.mParam2, true))
		{
			consoleLog("You can't use the " + lItem->getName() + " on the " + lTarget->getName() + ".");
			lSuccess = false;
		}
		else
		{
			consoleLog("There doesn't seem to be anything called " + aInstruction.mParam2 + " in here.");
			lSuccess = false;
		}
	}
	else
	{
		consoleLog("You don't have the " + aInstruction.mParam1 + " that you intend to use.");
		lSuccess = false;
	}

	return lSuccess;
}


bool Player::put(const Instruction& lInstruction)
{
	bool lSuccess;
	// Place an lItem inside another lItem (if allowed)

	// Try to get an Entity* with the requested aName from the Inventory (will be an Item).
	Entity* lItem = getChild(lInstruction.mParam1);
	if (lItem)
	{
		// Try to get a second Entity* with the requested aName form the Inventory (will also be an lItem).
		if (Entity* lContainerItem = getChild(lInstruction.mParam2))
		{
			ASSERT(mActionFactory);
			Action* lItemPut = mActionFactory->getAction(ActionType::ITEM_PUT, lItem, lContainerItem);
			if (lItemPut)
			{
				lItemPut->performAction();
				lSuccess = true;
			}
			else
			{
				consoleLog("You can't put the " + lItem->getName() + " in the " + lContainerItem->getName() + ".");
				lSuccess = false;
			}
		}
		else
		{
			consoleLog("There is no " + lInstruction.mParam2 + " in your inventory to put the " + lItem->getName() + " into.");
			lSuccess = false;
		}
	}
	else
	{
		consoleLog("You don't have the " + lInstruction.mParam1 + " that you intend to put into something else.");
		lSuccess = false;
	}

	return lSuccess;
}


void Player::updateLightStatus()
{
	mHasLight = false;

	for (Entity* lItem : mChildren)
	{
		if ((static_cast<Item*>(lItem))->hasLight())
		{
			mHasLight = true;
			break;
		}
	}
}
