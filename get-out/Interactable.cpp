#include "Interactable.h"

#include "EntityType.h"


Interactable::Interactable(int id, const std::string& name, const std::string& description, const std::string& inspectDescription, bool isVisibleInDark)
	: Entity(id, EntityType::INTERACTABLE, name, description, isVisibleInDark), m_inspectDescription(inspectDescription), m_isVisibleInDark(isVisibleInDark)
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


bool Interactable::canAddChild(const Entity* child) const
{
	return child->getType() == EntityType::ITEM;
}
