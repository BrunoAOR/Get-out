#include "EffectReplaceEntity.h"

#include <assert.h>
#include "Entity.h"


EffectReplaceEntity::EffectReplaceEntity(const std::string& effectDescription, Entity* entityToRemove, Entity* entityToAdd)
	: ActionEffect(effectDescription), m_entityToRemove(entityToRemove), m_entityToAdd(entityToAdd)
{
	assert(m_entityToRemove && m_entityToAdd);
}


EffectReplaceEntity::~EffectReplaceEntity()
{
}


void EffectReplaceEntity::doEffect() const
{
	if (m_entityToAdd != m_entityToRemove)
	{
		Entity* parent = m_entityToRemove->getParent();
		assert(parent);
		m_entityToRemove->setParent(nullptr);
		m_entityToAdd->setParent(parent);
	}
}
