#include "Interactable.h"

#include "EntityType.h"


Interactable::Interactable(int aId, const std::string& aName, const std::string& aDescription, const std::string& aInspectDescription, bool aIsVisibleInDark)
	: Entity(aId, EntityType::INTERACTABLE, aName, aDescription, aIsVisibleInDark), mInspectDescription(aInspectDescription), mIsVisibleInDark(aIsVisibleInDark)
{
}


Interactable::~Interactable()
{
}


std::string Interactable::getDetailedDescription() const
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


bool Interactable::canAddChild(const Entity* aChild) const
{
	return aChild->getType() == EntityType::ITEM;
}
