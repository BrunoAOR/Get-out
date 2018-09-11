#include "Item.h"

#include "EntityType.h"


Item::Item(int id, const std::string& name, const std::string& description, const std::string& inspectDescription, bool isVisibleInDark, bool hasLight)
	: Entity(id, EntityType::ITEM, name, description, isVisibleInDark), m_inspectDescription(inspectDescription), m_hasLight(hasLight)
{
}


Item::~Item()
{
}


std::string Item::getDescription() const
{
	std::string description = m_description;
	if (m_children.size() > 0)
	{
		description += " which contains:";
		for (auto child : m_children)
		{
			description += "\n    - " + child->getDescription();
		}
	}
	return description;
}


std::string Item::getDetailedDescription() const
{
	std::string description = m_inspectDescription;
	if (description == "")
	{
		description = "There is nothing special about the " + m_name + ".";
	}
	if (m_children.size() > 0)
	{
		description += "\nThe " + m_name + " contains:";
		for (auto child : m_children)
		{
			description += "\n    - " + child->getDescription();
		}
	}
	return description;
}


bool Item::hasLight() const
{
	return m_hasLight;
}


bool Item::canAddChild(const Entity* child) const
{
	return child->getType() == EntityType::ITEM;
}
