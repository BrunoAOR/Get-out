#include "Exit.h"
#include <assert.h>


Exit::Exit(EntityType type, std::string name, std::string description, Direction direction, bool isLocked, std::string lockedDescription, Room * targetRoom)
	: Entity(type, name, description), m_direction(direction), m_isLocked(isLocked), m_lockedDescription(lockedDescription), m_targetRoom(targetRoom)
{
	assert(m_targetRoom);
}


Exit::~Exit()
{
}


Direction Exit::getDirection() const
{
	return m_direction;
}


bool Exit::isLocked() const
{
	return m_isLocked;
}


std::string Exit::getLockedDescription() const
{
	return m_lockedDescription;
}


Room * Exit::getTargetRoom() const
{
	return m_targetRoom;
}


void Exit::unlock()
{
	m_isLocked = false;
}


bool Exit::canAddChild(Entity * child)
{
	return false;
}
