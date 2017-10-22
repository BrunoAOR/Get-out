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

bool Interactable::canAddChild(Entity * child)
{
	return child->getType() == EntityType::ITEM;
}
