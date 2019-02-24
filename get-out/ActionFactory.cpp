#include "ActionFactory.h"

#include "globals.h"
#include "Action.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "globals.h"
class Entity;


ActionFactory::ActionFactory(EntityFactory* aEntityFactory)
	: mEntityFactory(aEntityFactory)
{
	ASSERT(mEntityFactory);
}


ActionFactory::~ActionFactory()
{
	if (mActions.size() > 0)
	{
		close();
	}
}


void ActionFactory::close()
{
	for (Action* lAction : mActions)
	{
		delete lAction;
	}
	mActions.clear();
}


Action* ActionFactory::createAction(ActionType aType, const std::string& aDescription, const std::vector< ActionEffect* >& aEffects, bool aShouldDestroy, int aFirstEntityId, int aSecondEntityId)
{
	Entity* lFirstEntity = nullptr;
	if (aFirstEntityId >= 0)
	{
		lFirstEntity = mEntityFactory->getEntity(aFirstEntityId);
		ASSERT(lFirstEntity);
	}
	Entity* lSecondEntity = nullptr;
	if (aSecondEntityId >= 0)
	{
		lSecondEntity = mEntityFactory->getEntity(aSecondEntityId);
		ASSERT(lSecondEntity);
	}
	Action* lAction = new Action(this, aType, aDescription, aEffects, aShouldDestroy, lFirstEntity, lSecondEntity);
	mActions.push_back(lAction);
	return lAction;
}


Action* ActionFactory::getAction(ActionType aActionType, const Entity* lFirstEntity, const Entity* aSecondEntity) const
{
	if (lFirstEntity == nullptr && aSecondEntity == nullptr)
	{
		return nullptr;
	}

	for (auto lAction : mActions)
	{
		if (aActionType == lAction->getActionType() && lFirstEntity == lAction->getFirstEntity() && aSecondEntity == lAction->getSecondEntity())
		{
			return lAction;
		}
	}
	return nullptr;
}


void ActionFactory::removeAction(Action* lAction)
{
	ASSERT(lAction);
	auto lIt = std::find(mActions.begin(), mActions.end(), lAction);
	if (lIt != mActions.end())
	{
		mActions.erase(lIt);
		delete lAction;
	}
}
