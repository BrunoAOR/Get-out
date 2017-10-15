#include "Room.h"
#include <assert.h>
#include "EntityType.h"
#include "Direction.h"
#include "Exit.h"

Room::Room(EntityType type, std::string name, std::string description, bool isDark)
	: Entity(type, name, description), m_isDark(isDark)
{
}


Room::~Room()
{
	m_exits.clear();
}

Exit * Room::getExit(Direction direction)
{
	for (Exit* exit : m_exits)
	{
		if (exit->getDirection() == direction)
		{
			return exit;
		}
	}
	return nullptr;
}


bool Room::hasItem(Entity * item)
{
	for (Entity* child : m_children)
	{
		if (child == item)
		{
			return true;
		}
	}
	return false;
}


std::string Room::getDescription() const
{
	std::string description = m_name + "\n" + m_description;
	description += "\nIn the room you find the following:";
	for (Entity* child : m_children)
	{
		description += "\n  " + child->getDescription();
		if (child->getType() == EntityType::EXIT)
		{
			description += " to the " + getStringFromDirection(static_cast<Exit*>(child)->getDirection()) + ".";
		}
	}
	for (Exit* exit : m_exits)
	{
		description += "\n  " + exit->getDescription() + " to the " + getStringFromDirection(static_cast<Exit*>(exit)->getDirection()) + ".";
	}

	return description;
}


bool Room::canAddChild(Entity * child)
{
	return child->getType() == EntityType::EXIT
		|| child->getType() == EntityType::ITEM
		|| child->getType() == EntityType::INTERACTABLE;
}

void Room::addChild(Entity * child)
{
	if (child->getType() == EntityType::EXIT)
	{
		Exit* newExit = static_cast<Exit*>(child);
		for (Exit* exit : m_exits)
		{
			assert(exit->getDirection() != newExit->getDirection());
		}
		m_exits.push_back(static_cast<Exit*>(child));
	}
	else
	{
		m_children.push_back(child);
	}
}

void Room::removeChild(Entity * entity)
{
	if (entity->getType() == EntityType::EXIT)
	{
		Exit* exit = static_cast<Exit*>(entity);
		auto it = std::find(m_exits.begin(), m_exits.end(), exit);
		if (it != m_exits.end())
		{
			m_exits.erase(it);
		}
	}
	else
	{
		auto it = std::find(m_children.begin(), m_children.end(), entity);
		if (it != m_children.end())
		{
			m_children.erase(it);
		}
	}
}
