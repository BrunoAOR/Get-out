#include "Entity.h"
#include "EntityType.h"
#include "globals.h"
#include <assert.h>

Entity::Entity(EntityType type, std::string name, std::string description)
	: m_type(type), m_name(name), m_description(description)
{
}


Entity::~Entity()
{
	m_children.clear();
}


EntityType Entity::getType() const
{
	return m_type;
}


std::string Entity::getName() const
{
	return m_name;
}


std::string Entity::getDescription() const
{
	return m_description;
}


std::string Entity::getDetailedDescription() const
{
	return "";
}


bool Entity::hasChild(Entity * entity) const
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


Entity * Entity::getChild(const std::string & entityName, bool searchInChildren)
{
	for (Entity* child : m_children)
	{
		if (caselessEquals(child->m_name, entityName))
		{
			return child;
		}
		if (searchInChildren)
		{
			Entity* childFound = child->getChild(entityName, true);
			if (childFound)
			{
				return childFound;
			}
		}
	}
	return nullptr;
}


Entity * Entity::getParent() const
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


bool Entity::canAddChild(Entity * child)
{
	return true;
}


void Entity::addChild(Entity * child)
{
	m_children.push_back(child);
}


void Entity::removeChild(Entity * entity)
{
	auto it = std::find(m_children.begin(), m_children.end(), entity);
	if (it != m_children.end())
	{
		m_children.erase(it);
	}
}
