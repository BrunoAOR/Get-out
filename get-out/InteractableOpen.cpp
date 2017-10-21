#include "InteractableOpen.h"
#include "ActionEffect.h"
#include "globals.h"
#include "World.h"
#include <assert.h>


InteractableOpen::InteractableOpen(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Interactable* interactable)
	: m_description(description), m_effects(effects), m_shouldDestroy(shouldDestroy), m_interactable(interactable)
{
	assert(m_interactable);
}


InteractableOpen::~InteractableOpen()
{
	for (auto effect : m_effects)
	{
		delete effect;
	}
	m_effects.clear();
}

const Interactable * InteractableOpen::getInteractable() const
{
	return m_interactable;
}

std::string InteractableOpen::getDescription() const
{
	return m_description;
}


void InteractableOpen::performEffects()
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
		world->removeInteractableOpen(this);
	}
}
