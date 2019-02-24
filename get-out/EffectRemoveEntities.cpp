#include "EffectRemoveEntities.h"

#include "Entity.h"


EffectRemoveEntities::EffectRemoveEntities(const std::string& aEffectDescription, const std::vector< Entity* >& aEntitiesToRemove)
	: ActionEffect(aEffectDescription), mEntitiesToRemove(aEntitiesToRemove)
{
}


EffectRemoveEntities::~EffectRemoveEntities()
{
}


void EffectRemoveEntities::doEffect() const
{
	for (Entity* lEntity : mEntitiesToRemove)
	{
		lEntity->setParent(nullptr);
	}
}
