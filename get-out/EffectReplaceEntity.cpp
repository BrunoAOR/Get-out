#include "EffectReplaceEntity.h"
#include <assert.h>


EffectReplaceEntity::EffectReplaceEntity(std::string effectDescription, Entity* entityToRemove, Entity* entityToAdd)
	: ActionEffect(effectDescription), m_entityToRemove(entityToRemove), m_entityToAdd(entityToAdd)
{
	assert(m_entityToRemove && m_entityToAdd);
}


EffectReplaceEntity::~EffectReplaceEntity()
{
}

void EffectReplaceEntity::doEffect()
{
	if (m_entityToAdd == m_entityToRemove)
	{
		return;
	}
	Entity* parent = m_entityToRemove->getParent();
	assert(parent);
	m_entityToRemove->setParent(nullptr);
	m_entityToAdd->setParent(parent);
}
