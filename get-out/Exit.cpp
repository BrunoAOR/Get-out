#include "Exit.h"


Exit::Exit(EntityType type, std::string name, std::string description, Direction direction, bool isLocked, std::string lockedDescription, Room * targetRoom)
	: Entity(type, name, description), m_direction(direction), m_isLocked(isLocked), m_lockedDescription(lockedDescription), m_targetRoom(targetRoom)
{
}


Exit::~Exit()
{
}


Direction Exit::getDirection() const
{
	return m_direction;
}


bool Exit::canAddChild(Entity * child)
{
	return false;
}
