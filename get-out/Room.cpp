#include "Room.h"
#include "EntityType.h"
#include "Direction.h"
#include "Exit.h"

Room::Room(EntityType type, std::string name, std::string description, bool isDark)
	: Entity(type, name, description), m_isDark(isDark)
{
}


Room::~Room()
{
}


std::string Room::getDescription() const
{
	std::string exits;
	std::string description = m_description;
	description += "\nIn the room you find the following:";
	for (Entity* child : m_children)
	{
		if (child->getType() != EntityType::PLAYER)
		{
			description += "\n  " + child->getDescription();
		}
		if (child->getType() == EntityType::EXIT)
		{
			description += " to the " + getStringFromDirection(static_cast<Exit*>(child)->getDirection()) + ".";
		}
	}
	description += "\n";
	return description;
}


bool Room::canAddChild(Entity * child)
{
	return child->getType() != EntityType::ROOM;
}
