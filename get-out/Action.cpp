#include "Action.h"
#include "World.h"
#include "ActionEffect.h"
#include "ActionType.h"
#include "globals.h"
#include <assert.h>


#include "Item.h"
#include "Interactable.h"
Action::Action(ActionType type, std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Entity * firstEntity, Entity * secondEntity)
	: m_type(type), m_description(description), m_effects(effects), m_shouldDestroy(shouldDestroy), m_firstEntity(firstEntity), m_secondEntity(secondEntity)
{
	switch (type)
	{
	case ActionType::InteractableOpen:
		assert(m_firstEntity && typeid(*m_firstEntity) == typeid(Interactable));
		break;
	case ActionType::ItemPut:
		assert(m_firstEntity && typeid(*m_firstEntity) == typeid(Item) && m_secondEntity && typeid(*m_secondEntity) == typeid(Item));
		break;
	case ActionType::ItemUse:
		assert(m_firstEntity && typeid(*m_firstEntity) == typeid(Item) && m_secondEntity && typeid(*m_secondEntity) == typeid(Interactable));
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


std::string Action::getDescription() const
{
	return m_description;
}


const Entity * Action::getFirstEntity() const
{
	return m_firstEntity;
}


const Entity * Action::getSecondEntity() const
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
	consoleLog(message);
	if (m_shouldDestroy)
	{
		world->removeAction(this);
	}
}
