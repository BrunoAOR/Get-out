#include "EffectRemoveEntities.h"

#include "Entity.h"


EffectRemoveEntities::EffectRemoveEntities(const std::string& effectDescription, const std::vector<Entity*>& entitiesToRemove)
	: ActionEffect(effectDescription), m_entitiesToRemove(entitiesToRemove)
{
}


EffectRemoveEntities::~EffectRemoveEntities()
{
}


void EffectRemoveEntities::doEffect() const
{
	for (Entity* entity : m_entitiesToRemove)
	{
		entity->setParent(nullptr);
	}
}
