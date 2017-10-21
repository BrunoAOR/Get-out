#include "ItemUse.h"
#include "ActionType.h"
#include "Item.h"
#include "Interactable.h"

ItemUse::ItemUse(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Item * item, Interactable * interactable)
	: Action(ActionType::ItemUse, description, effects, shouldDestroy, item, interactable)
{
}

ItemUse::~ItemUse()
{
}
