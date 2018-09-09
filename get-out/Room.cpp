#include "Room.h"

#include <assert.h>
#include "EntityType.h"
#include "Direction.h"
#include "Exit.h"
#include "globals.h"


Room::Room(int id, const std::string& name, const std::string& description, bool isDark)
	: Entity(id, EntityType::ROOM, name, description, true), m_isDark(isDark)
{
}


Room::~Room()
{
	m_exits.clear();
}


Exit* Room::getExit(Direction direction)
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


std::string Room::getDescription() const
{
	std::string description = m_name + "\n" + m_description;
	description += "\nIn the room you find the following:";
	for (Exit* exit : m_exits)
	{
		description += "\n  " + exit->getDescription() + " to the " + getStringFromDirection(static_cast<Exit*>(exit)->getDirection());
	}
	for (Entity* child : m_children)
	{
		description += "\n  " + child->getDescription();
	}
	return description;
}


std::string Room::getDescriptionInDarkness() const
{
	if (!m_isDark)
	{
		return getDescription();
	}

	std::string description = m_name + "\n" + m_description;
	description += "\nThe room is almost in full darkness, but you can identify the following things:";
	for (Exit* exit : m_exits)
	{
		description += "\n  " + exit->getDescription() + " to the " + getStringFromDirection(static_cast<Exit*>(exit)->getDirection());
	}
	for (Entity* child : m_children)
	{
		if (child->isVisibleInDark())
		{
			description += "\n  " + child->getDescription();
		}
	}
	return description;
}


Entity* Room::getChildInDarkness(const std::string& entityName, bool searchInChildren) const
{
	Entity* queryChild = nullptr;

	if (!m_isDark)
	{
		queryChild = getChild(entityName, searchInChildren);
	}
	else
	{

		for (Entity* child : m_children)
		{
			if (child->isVisibleInDark())
			{
				if (caselessEquals(child->getName(), entityName))
				{
					queryChild = child;
					break;
				}
				if (searchInChildren)
				{
					Entity* childFound = child->getChild(entityName, true);
					if (childFound)
					{
						queryChild = childFound;
						break;
					}
				}
			}
		}
	}

	return queryChild;
}


bool Room::canAddChild(const Entity* child) const
{
	return child->getType() == EntityType::EXIT
		|| child->getType() == EntityType::ITEM
		|| child->getType() == EntityType::INTERACTABLE;
}


void Room::addChild(Entity* child)
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


void Room::removeChild(const Entity* entity)
{
	if (entity->getType() == EntityType::EXIT)
	{
		const Exit* exit = static_cast<const Exit*>(entity);
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
