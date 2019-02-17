#include "EffectAddEntitiesToRoom.h"

#include <assert.h>
#include "Room.h"
#include "globals.h"


EffectAddEntitiesToRoom::EffectAddEntitiesToRoom(const std::string& aEffectDescription, const std::vector<Entity*>& aEntitiesToAdd, Room* aTargetRoom)
	: ActionEffect(aEffectDescription), mEntitiesToAdd(aEntitiesToAdd), mTargetRoom(aTargetRoom)
{
	assert(mTargetRoom);
}


EffectAddEntitiesToRoom::~EffectAddEntitiesToRoom()
{
}


void EffectAddEntitiesToRoom::doEffect() const
{
	bool lSuccess = true;
	for (Entity* lEntity : mEntitiesToAdd)
	{
		lSuccess &= lEntity->setParent(mTargetRoom);
	}
	if (!lSuccess)
	{
		OutputLog("ERROR: Effect of type EffectAddEntitiesToRoom couldn't be properly applied!");
		assert(false);
	}
}
