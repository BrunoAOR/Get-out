#include "ActionType.h"

#include "globals.h"


ActionType getActionTypeFromString(const std::string& aText)
{
	if (caselessEquals(aText, "GO"))				return ActionType::GO;
	if (caselessEquals(aText, "TAKE"))				return ActionType::TAKE;
	if (caselessEquals(aText, "DROP"))				return ActionType::DROP;
	if (caselessEquals(aText, "INTERACTABLE_OPEN"))	return ActionType::INTERACTABLE_OPEN;
	if (caselessEquals(aText, "ITEM_USE"))			return ActionType::ITEM_USE;
	if (caselessEquals(aText, "ITEM_PUT"))			return ActionType::ITEM_PUT;
	return ActionType::_UNDEFINED;
}
