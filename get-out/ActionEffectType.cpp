#include "ActionEffectType.h"

#include "globals.h"


ActionEffectType getActionEffectTypeFromString(const std::string& text)
{
	if (caselessEquals(text, "ADD_ENTITIES"))		return ActionEffectType::ADD_ENTITIES;
	if (caselessEquals(text, "GAME_END"))			return ActionEffectType::GAME_END;
	if (caselessEquals(text, "LOCK_EXIT"))			return ActionEffectType::LOCK_EXIT;
	if (caselessEquals(text, "PLACE_ITEM_IN_ITEM"))	return ActionEffectType::PLACE_ITEM_IN_ITEM;
	if (caselessEquals(text, "REMOVE_ENTITIES"))	return ActionEffectType::REMOVE_ENTITIES;
	if (caselessEquals(text, "REPLACE_ENTITY"))		return ActionEffectType::REPLACE_ENTITY;
	if (caselessEquals(text, "UNLOCK_EXIT"))		return ActionEffectType::UNLOCK_EXIT;
	return ActionEffectType::_UNDEFINED;
}
