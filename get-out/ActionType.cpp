#include "ActionType.h"

#include "globals.h"


ActionType getActionTypeFromString(const std::string& text)
{
	if (caselessEquals(text, "GO"))					return ActionType::GO;
	if (caselessEquals(text, "TAKE"))				return ActionType::TAKE;
	if (caselessEquals(text, "DROP"))				return ActionType::DROP;
	if (caselessEquals(text, "INTERACTABLE_OPEN"))	return ActionType::INTERACTABLE_OPEN;
	if (caselessEquals(text, "ITEM_USE"))			return ActionType::ITEM_USE;
	if (caselessEquals(text, "ITEM_PUT"))			return ActionType::ITEM_PUT;
	return ActionType::_UNDEFINED;
}
