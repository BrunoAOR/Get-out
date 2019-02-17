#include "ActionEffectType.h"

#include "globals.h"


ActionEffectType getActionEffectTypeFromString(const std::string& aText)
{
	if (caselessEquals(aText, "ADD_ENTITIES"))			return ActionEffectType::ADD_ENTITIES;
	if (caselessEquals(aText, "GAME_END"))				return ActionEffectType::GAME_END;
	if (caselessEquals(aText, "LOCK_EXIT"))				return ActionEffectType::LOCK_EXIT;
	if (caselessEquals(aText, "PLACE_ITEM_IN_ITEM"))	return ActionEffectType::PLACE_ITEM_IN_ITEM;
	if (caselessEquals(aText, "REMOVE_ENTITIES"))		return ActionEffectType::REMOVE_ENTITIES;
	if (caselessEquals(aText, "REPLACE_ENTITY"))		return ActionEffectType::REPLACE_ENTITY;
	if (caselessEquals(aText, "UNLOCK_EXIT"))			return ActionEffectType::UNLOCK_EXIT;
	return ActionEffectType::_UNDEFINED;
}
