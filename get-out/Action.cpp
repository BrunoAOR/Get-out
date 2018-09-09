#include "Action.h"

#include <assert.h>
#include "ActionEffect.h"
#include "ActionType.h"
#include "EntityType.h"
#include "globals.h"
#include "Item.h"
#include "Interactable.h"
#include "Room.h"


Action::Action(ActionFactory* actionFactory, ActionType type, const std::string& description, const std::vector<ActionEffect*>& effects, bool shouldDestroy, Entity * firstEntity, Entity * secondEntity)
	: m_actionFactory(actionFactory), m_type(type), m_description(description), m_effects(effects), m_shouldDestroy(shouldDestroy), m_firstEntity(firstEntity), m_secondEntity(secondEntity)
{
	assert(m_actionFactory);
	switch (type)
	{
	case ActionType::GO:
		assert((m_firstEntity || m_secondEntity) && (m_firstEntity ? m_firstEntity->getType() == EntityType::ROOM : true) && (m_secondEntity ? m_secondEntity->getType() == EntityType::ROOM : true));
		break;
	case ActionType::TAKE:
		assert(m_firstEntity && m_firstEntity->getType() == EntityType::ITEM && !m_secondEntity);
		break;
	case ActionType::DROP:
		assert(m_firstEntity && m_firstEntity->getType() == EntityType::ITEM && !m_secondEntity);
		break;
	case ActionType::INTERACTABLE_OPEN:
		assert(m_firstEntity && m_firstEntity->getType() == EntityType::INTERACTABLE && !m_secondEntity);
		break;
	case ActionType::ITEM_USE:
		assert(m_firstEntity && m_firstEntity->getType() == EntityType::ITEM && m_secondEntity && m_secondEntity->getType() == EntityType::INTERACTABLE);
		break;
	case ActionType::ITEM_PUT:
		assert(m_firstEntity && m_firstEntity->getType() == EntityType::ITEM && m_secondEntity && m_secondEntity->getType() == EntityType::ITEM);
		break;
	}
}


Action::~Action()
{
	for (auto effect : m_effects)
	{
		delete effect;
	}
	m_effects.clear();
}


ActionType Action::getActionType() const
{
	return m_type;
}


const std::string& Action::getDescription() const
{
	return m_description;
}


const Entity* Action::getFirstEntity() const
{
	return m_firstEntity;
}


const Entity* Action::getSecondEntity() const
{
	return m_secondEntity;
}


void Action::performAction()
{
	std::string message = m_description;
	for (ActionEffect* effect : m_effects)
	{
		effect->doEffect();
		if (effect->getEffectDescription() != "")
		{
			message += "\n" + effect->getEffectDescription();
		}
	}
	if (message != "")
	{
		consoleLog(message);
	}
	if (m_shouldDestroy)
	{
		m_actionFactory->removeAction(this);
	}
}
