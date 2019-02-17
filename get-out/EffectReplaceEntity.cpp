#include "EffectReplaceEntity.h"

#include <assert.h>
#include "Entity.h"


EffectReplaceEntity::EffectReplaceEntity(const std::string& aEffectDescription, Entity* aEntityToRemove, Entity* aEntityToAdd)
	: ActionEffect(aEffectDescription), mEntityToRemove(aEntityToRemove), mEntityToAdd(aEntityToAdd)
{
	assert(mEntityToRemove && mEntityToAdd);
}


EffectReplaceEntity::~EffectReplaceEntity()
{
}


void EffectReplaceEntity::doEffect() const
{
	if (mEntityToAdd != mEntityToRemove)
	{
		Entity* lParent = mEntityToRemove->getParent();
		assert(lParent);
		mEntityToRemove->setParent(nullptr);
		mEntityToAdd->setParent(lParent);
	}
}
