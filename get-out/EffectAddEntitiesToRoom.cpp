#include "EffectAddEntitiesToRoom.h"
#include "Room.h"
#include "globals.h"
#include <assert.h>

EffectAddEntitiesToRoom::EffectAddEntitiesToRoom(std::string effectDescription, std::vector<Entity*> entitiesToAdd, Room * targetRoom)
	: ActionEffect(effectDescription), m_entitiesToAdd(entitiesToAdd), m_targetRoom(targetRoom)
{
	assert(m_targetRoom);
}

EffectAddEntitiesToRoom::~EffectAddEntitiesToRoom()
{
	m_entitiesToAdd.clear();
}

void EffectAddEntitiesToRoom::doEffect()
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
