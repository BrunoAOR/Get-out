#include "Interactable.h"
#include "EntityType.h"


Interactable::Interactable(EntityType type, std::string name, std::string description, std::string inspectDescription, bool isLocked, bool isVisibleInDark)
	: Entity(type, name, description), m_inspectDescription(inspectDescription), m_isLocked(isLocked), m_isVisibleInDark(isVisibleInDark)
{
}


Interactable::~Interactable()
{
}

std::string Interactable::getDetailedDescription() const
{
	return m_inspectDescription;
}

bool Interactable::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}
