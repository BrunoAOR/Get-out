#include "EffectAddEntitiesToRoom.h"

#include <assert.h>
#include "Room.h"
#include "globals.h"


EffectAddEntitiesToRoom::EffectAddEntitiesToRoom(const std::string& effectDescription, std::vector<Entity*> entitiesToAdd, Room * targetRoom)
	: ActionEffect(effectDescription), m_entitiesToAdd(entitiesToAdd), m_targetRoom(targetRoom)
{
	assert(m_targetRoom);
}


EffectAddEntitiesToRoom::~EffectAddEntitiesToRoom()
{
}


void EffectAddEntitiesToRoom::doEffect() const
{
	bool success = true;
	for (Entity* entity : m_entitiesToAdd)
	{
		success &= entity->setParent(m_targetRoom);
	}
	if (!success)
	{
		OutputLog("ERROR: Effect of type EffectAddEntitiesToRoom couldn't be properly applied!");
		assert(false);
	}
}
