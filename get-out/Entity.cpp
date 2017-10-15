#include "Entity.h"
#include "EntityType.h"


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
	if (parent == nullptr || !parent->canAddChild(this))
	{
		return false;
	}
	if (m_parent != nullptr)
	{
		m_parent->removeChild(this);
		m_parent = nullptr;
	}
	if (parent != nullptr)
	{
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
