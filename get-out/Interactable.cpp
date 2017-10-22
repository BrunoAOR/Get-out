#include "Interactable.h"
#include "EntityType.h"


Interactable::Interactable(std::string name, std::string m_description, std::string inspectDescription, bool isVisibleInDark)
	: Entity(EntityType::INTERACTABLE, name, m_description, isVisibleInDark), m_inspectDescription(inspectDescription), m_isVisibleInDark(isVisibleInDark)
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
