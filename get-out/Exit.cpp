#include "Exit.h"

#include <assert.h>
#include "EntityType.h"


Exit::Exit(int id, const std::string& name, const std::string& description, Direction direction, bool isLocked, const std::string& lockedDescription, Room * targetRoom)
	: Entity(id, EntityType::EXIT, name, description, true), m_direction(direction), m_isLocked(isLocked), m_lockedDescription(lockedDescription), m_targetRoom(targetRoom)
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


Room* Exit::getTargetRoom() const
{
	return m_targetRoom;
}


void Exit::lock()
{
	m_isLocked = true;
}


void Exit::unlock()
{
	m_isLocked = false;
}


bool Exit::canAddChild(const Entity* child) const
{
	return false;
}
