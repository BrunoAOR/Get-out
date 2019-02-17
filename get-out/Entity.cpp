#include "Entity.h"

#include <assert.h>
#include "EntityType.h"
#include "globals.h"


Entity::Entity(int aId, EntityType aType, const std::string& aName, const std::string& aDescription, bool aIsVisibleInDark)
	: mId(aId), mType(aType), mName(aName), mDescription(aDescription), mIsVisibleInDark(aIsVisibleInDark)
{
}


Entity::~Entity()
{
}


EntityType Entity::getType() const
{
	return mType;
}


const std::string& Entity::getName() const
{
	return mName;
}


std::string Entity::getDescription() const
{
	return mDescription;
}


bool Entity::isVisibleInDark() const
{
	return mIsVisibleInDark;
}


std::string Entity::getDetailedDescription() const
{
	return "There is nothing special about the " + mName + ".";
}


bool Entity::hasChild(Entity* aEntity) const
{
	for (Entity* lChild : mChildren)
	{
		if (lChild == aEntity)
		{
			return true;
		}
	}
	return false;
}


Entity* Entity::getChild(const std::string& aEntityName, bool aSearchInChildren) const
{
	Entity* lQueryChild = nullptr;

	for (Entity* lChild : mChildren)
	{
		if (caselessEquals(lChild->mName, aEntityName))
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

	return lQueryChild;
}


Entity* Entity::getParent() const
{
	return mParent;
}


bool Entity::setParent(Entity* aParent)
{
	if (mParent == aParent)
	{
		return true;
	}
	if (mParent != nullptr)
	{
		mParent->removeChild(this);
		mParent = nullptr;
	}
	if (aParent != nullptr)
	{
		assert(aParent->canAddChild(this));
		aParent->addChild(this);
		mParent = aParent;
	}
	return true;
}


bool Entity::canAddChild(const Entity* aChild) const
{
	return false;
}


void Entity::addChild(Entity* aChild)
{
	mChildren.push_back(aChild);
}


void Entity::removeChild(const Entity* aEntity)
{
	auto lIt = std::find(mChildren.begin(), mChildren.end(), aEntity);
	if (lIt != mChildren.end())
	{
		mChildren.erase(lIt);
	}
}
