#include "Item.h"
#include "EntityType.h"


Item::Item(std::string name, std::string m_description, std::string inspectDescription, bool isVisibleInDark, bool hasLight)
	: Entity(EntityType::ITEM, name, m_description, isVisibleInDark), m_inspectDescription(inspectDescription), m_hasLight(hasLight)
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
	return m_inspectDescription;
}


bool Item::hasLight() const
{
	return m_hasLight;
}


bool Item::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}
