#include "Room.h"

#include <assert.h>
#include "EntityType.h"
#include "Direction.h"
#include "Exit.h"
#include "globals.h"


Room::Room(int aId, const std::string& aName, const std::string& aDescription, bool aIsDark)
	: Entity(aId, EntityType::ROOM, aName, aDescription, true), mIsDark(aIsDark)
{
}


Room::~Room()
{
	mExits.clear();
}


Exit* Room::getExit(Direction aDirection)
{
	for (Exit* exit : mExits)
	{
		if (exit->getDirection() == aDirection)
		{
			return exit;
		}
	}
	return nullptr;
}


std::string Room::getDescription() const
{
	std::string lDescription = mName + "\n" + mDescription;
	lDescription += "\nIn the room you find the following:";
	for (Exit* lExit : mExits)
	{
		lDescription += "\n  " + lExit->getDescription() + " to the " + getStringFromDirection(static_cast<Exit*>(lExit)->getDirection());
	}
	for (Entity* lChild : mChildren)
	{
		lDescription += "\n  " + lChild->getDescription();
	}
	return lDescription;
}


std::string Room::getDescriptionInDarkness() const
{
	if (!mIsDark)
	{
		return getDescription();
	}

	std::string lDescription = mName + "\n" + mDescription;
	lDescription += "\nThe room is almost in full darkness, but you can identify the following things:";
	for (Exit* lExit : mExits)
	{
		lDescription += "\n  " + lExit->getDescription() + " to the " + getStringFromDirection(static_cast<Exit*>(lExit)->getDirection());
	}
	for (Entity* lChild : mChildren)
	{
		if (lChild->isVisibleInDark())
		{
			lDescription += "\n  " + lChild->getDescription();
		}
	}
	return lDescription;
}


Entity* Room::getChildInDarkness(const std::string& aEntityName, bool aSearchInChildren) const
{
	Entity* lQueryChild = nullptr;

	if (!mIsDark)
	{
		lQueryChild = getChild(aEntityName, aSearchInChildren);
	}
	else
	{

		for (Entity* lChild : mChildren)
		{
			if (lChild->isVisibleInDark())
			{
				if (caselessEquals(lChild->getName(), aEntityName))
				{
					lQueryChild = lChild;
					break;
				}
				if (aSearchInChildren)
				{
					Entity* lChildFound = lChild->getChild(aEntityName, true);
					if (lChildFound)
					{
						lQueryChild = lChildFound;
						break;
					}
				}
			}
		}
	}

	return lQueryChild;
}


bool Room::canAddChild(const Entity* aChild) const
{
	return aChild->getType() == EntityType::EXIT
		|| aChild->getType() == EntityType::ITEM
		|| aChild->getType() == EntityType::INTERACTABLE;
}


void Room::addChild(Entity* aChild)
{
	if (aChild->getType() == EntityType::EXIT)
	{
		Exit* lNewExit = static_cast<Exit*>(aChild);
		for (Exit* lExit : mExits)
		{
			assert(lExit->getDirection() != lNewExit->getDirection());
		}
		mExits.push_back(static_cast<Exit*>(aChild));
	}
	else
	{
		mChildren.push_back(aChild);
	}
}


void Room::removeChild(const Entity* aEntity)
{
	if (aEntity->getType() == EntityType::EXIT)
	{
		const Exit* lExit = static_cast<const Exit*>(aEntity);
		auto lIt = std::find(mExits.begin(), mExits.end(), lExit);
		if (lIt != mExits.end())
		{
			mExits.erase(lIt);
		}
	}
	else
	{
		auto lIt = std::find(mChildren.begin(), mChildren.end(), aEntity);
		if (lIt != mChildren.end())
		{
			mChildren.erase(lIt);
		}
	}
}
