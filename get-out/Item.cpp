#include "Item.h"

#include "EntityType.h"


Item::Item(int aId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark, bool aHasLight)
	: Entity(aId, EntityType::ITEM, aName, aDescription, aIsVisibleInDark), mInspectDescription(aInspectDescription), mHasLight(aHasLight)
{
}


Item::~Item()
{
}


std::string Item::getDescription() const
{
	std::string lDescription = mDescription;
	if (mChildren.size() > 0)
	{
		lDescription += " which contains:";
		for (auto lChild : mChildren)
		{
			lDescription += "\n    - " + lChild->getDescription();
		}
	}
	return lDescription;
}


std::string Item::getDetailedDescription() const
{
	std::string lDescription = mInspectDescription;
	if (lDescription == "")
	{
		lDescription = "There is nothing special about the " + mName + ".";
	}
	if (mChildren.size() > 0)
	{
		lDescription += "\nThe " + mName + " contains:";
		for (auto lChild : mChildren)
		{
			lDescription += "\n    - " + lChild->getDescription();
		}
	}
	return lDescription;
}


bool Item::hasLight() const
{
	return mHasLight;
}


bool Item::canAddChild(const Entity* aChild) const
{
	return aChild->getType() == EntityType::ITEM;
}
