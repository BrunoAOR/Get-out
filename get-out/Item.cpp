#include "Item.h"
#include "EntityType.h"


Item::Item(std::string name, std::string m_description, std::string inspectDescription, bool hasLight)
	: Entity(EntityType::ITEM, name, m_description), m_inspectDescription(inspectDescription), m_hasLight(hasLight)
{
}


Item::~Item()
{
}

std::string Item::getDetailedDescription() const
{
	return m_inspectDescription;
}

bool Item::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}
