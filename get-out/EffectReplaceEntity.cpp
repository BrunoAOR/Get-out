#include "EffectReplaceEntity.h"

#include "globals.h"
#include "Entity.h"


EffectReplaceEntity::EffectReplaceEntity(const std::string& aEffectDescription, Entity* aEntityToRemove, Entity* aEntityToAdd)
	: ActionEffect(aEffectDescription), mEntityToRemove(aEntityToRemove), mEntityToAdd(aEntityToAdd)
{
	ASSERT(mEntityToRemove && mEntityToAdd);
}


EffectReplaceEntity::~EffectReplaceEntity()
{
}


void EffectReplaceEntity::doEffect() const
{
	if (mEntityToAdd != mEntityToRemove)
	{
		Entity* lParent = mEntityToRemove->getParent();
		ASSERT(lParent);
		mEntityToRemove->setParent(nullptr);
		bool lSuccess = mEntityToAdd->setParent(lParent);
		ASSERT(lSuccess);
	}
}
