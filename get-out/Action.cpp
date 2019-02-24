#include "Action.h"

#include "globals.h"
#include "ActionEffect.h"
#include "ActionType.h"
#include "EntityType.h"
#include "globals.h"
#include "Item.h"
#include "Interactable.h"
#include "Room.h"


Action::Action(ActionFactory* aActionFactory, ActionType aType, const std::string& aDescription, const std::vector< ActionEffect* >& aEffects, bool aShouldDestroy, Entity * aFirstEntity, Entity * aSecondEntity)
	: mActionFactory(aActionFactory), mType(aType), mDescription(aDescription), mEffects(aEffects), mShouldDestroy(aShouldDestroy), mFirstEntity(aFirstEntity), mSecondEntity(aSecondEntity)
{
	ASSERT(mActionFactory);
	switch (aType)
	{
	case ActionType::GO:
		ASSERT((mFirstEntity || mSecondEntity) && (mFirstEntity ? mFirstEntity->getType() == EntityType::ROOM : true) && (mSecondEntity ? mSecondEntity->getType() == EntityType::ROOM : true));
		break;
	case ActionType::TAKE:
		ASSERT(mFirstEntity && mFirstEntity->getType() == EntityType::ITEM && !mSecondEntity);
		break;
	case ActionType::DROP:
		ASSERT(mFirstEntity && mFirstEntity->getType() == EntityType::ITEM && !mSecondEntity);
		break;
	case ActionType::INTERACTABLE_OPEN:
		ASSERT(mFirstEntity && mFirstEntity->getType() == EntityType::INTERACTABLE && !mSecondEntity);
		break;
	case ActionType::ITEM_USE:
		ASSERT(mFirstEntity && mFirstEntity->getType() == EntityType::ITEM && mSecondEntity && mSecondEntity->getType() == EntityType::INTERACTABLE);
		break;
	case ActionType::ITEM_PUT:
		ASSERT(mFirstEntity && mFirstEntity->getType() == EntityType::ITEM && mSecondEntity && mSecondEntity->getType() == EntityType::ITEM);
		break;
	}
}


Action::~Action()
{
	for (auto lEffect : mEffects)
	{
		delete lEffect;
	}
	mEffects.clear();
}


ActionType Action::getActionType() const
{
	return mType;
}


const std::string& Action::getDescription() const
{
	return mDescription;
}


const Entity* Action::getFirstEntity() const
{
	return mFirstEntity;
}


const Entity* Action::getSecondEntity() const
{
	return mSecondEntity;
}


void Action::performAction()
{
	std::string lMessage = mDescription;
	for (ActionEffect* lEffect : mEffects)
	{
		lEffect->doEffect();
		const std::string& lEffectDescription = lEffect->getEffectDescription();
		if (lEffectDescription != "")
		{
			lMessage += "\n" + lEffectDescription;
		}
	}
	if (lMessage != "")
	{
		consoleLog(lMessage);
	}
	if (mShouldDestroy)
	{
		mActionFactory->removeAction(this);
	}
}
