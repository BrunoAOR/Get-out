#include "ActionFactory.h"
#include "Action.h"
#include "EntityFactory.h"
#include "Entity.h"
#include "globals.h"
#include <assert.h>
class Entity;


ActionFactory::ActionFactory(EntityFactory* entityFactory)
	: m_entityFactory(entityFactory)
{
	assert(m_entityFactory);
}


ActionFactory::~ActionFactory()
{
	if (m_actions.size() > 0)
	{
		close();
	}
}


void ActionFactory::close()
{
	for (Action* action : m_actions)
	{
		delete action;
	}
	m_actions.clear();
}


Action * ActionFactory::createAction(ActionType type, const std::string& description, std::vector<ActionEffect*> effects, bool shouldDestroy, int firstEntityId, int secondEntityId)
{
	Entity* firstEntity = m_entityFactory->getEntity(firstEntityId);
	assert(firstEntity);
	Entity* secondEntity = nullptr;
	if (secondEntityId >= 0)
	{
		secondEntity = m_entityFactory->getEntity(secondEntityId);
		assert(secondEntity);
	}
	Action* action = new Action(type, description, effects, shouldDestroy, firstEntity, secondEntity);
	m_actions.push_back(action);
	return action;
}


Action * ActionFactory::getAction(ActionType actionType, const Entity * firstEntity, const Entity * secondEntity)
{
	if (firstEntity == nullptr)
	{
		return nullptr;
	}

	for (auto action : m_actions)
	{
		if (actionType == action->getActionType() && firstEntity == action->getFirstEntity() && secondEntity == action->getSecondEntity())
		{
			return action;
		}
	}
	return nullptr;
}


void ActionFactory::removeAction(Action * action)
{
	auto it = std::find(m_actions.begin(), m_actions.end(), action);
	assert(it != m_actions.end());
	m_actions.erase(it);
	delete action;
}
