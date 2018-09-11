#include "Entity.h"

#include <assert.h>
#include "EntityType.h"
#include "globals.h"


Entity::Entity(int id, EntityType type, const std::string& name, const std::string& description, bool isVisibleInDark)
	: m_id(id), m_type(type), m_name(name), m_description(description), m_isVisibleInDark(isVisibleInDark)
{
}


Entity::~Entity()
{
}


EntityType Entity::getType() const
{
	return m_type;
}


const std::string& Entity::getName() const
{
	return m_name;
}


std::string Entity::getDescription() const
{
	return m_description;
}


bool Entity::isVisibleInDark() const
{
	return m_isVisibleInDark;
}


std::string Entity::getDetailedDescription() const
{
	return "There is nothing special about the " + m_name + ".";
}


bool Entity::hasChild(Entity* entity) const
{
	for (Entity* child : m_children)
	{
		if (child == entity)
		{
			return true;
		}
	}
	return false;
}


Entity* Entity::getChild(const std::string& entityName, bool searchInChildren) const
{
	Entity* queryChild = nullptr;

	for (Entity* child : m_children)
	{
		if (caselessEquals(child->m_name, entityName))
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

	return queryChild;
}


Entity* Entity::getParent() const
{
	return m_parent;
}


bool Entity::setParent(Entity * parent)
{
	if (m_parent == parent)
	{
		return true;
	}
	if (m_parent != nullptr)
	{
		m_parent->removeChild(this);
		m_parent = nullptr;
	}
	if (parent != nullptr)
	{
		assert(parent->canAddChild(this));
		parent->addChild(this);
		m_parent = parent;
	}
	return true;
}


bool Entity::canAddChild(const Entity* child) const
{
	return false;
}


void Entity::addChild(Entity* child)
{
	m_children.push_back(child);
}


void Entity::removeChild(const Entity* entity)
{
	auto it = std::find(m_children.begin(), m_children.end(), entity);
	if (it != m_children.end())
	{
		m_children.erase(it);
	}
}
