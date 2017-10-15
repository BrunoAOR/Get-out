#include "Item.h"
#include "EntityType.h"


Item::Item(EntityType type, std::string name, std::string description, std::string inspectDescription, bool hasLight)
	: Entity(type, name, description), m_inspectDescription(inspectDescription), m_hasLight(hasLight)
{
}


Item::~Item()
{
}

bool Item::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}
