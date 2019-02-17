#ifndef H_ACTION_EFFECT_TYPE
#define H_ACTION_EFFECT_TYPE

#include <string>


enum class ActionEffectType
{
	_UNDEFINED,
	ADD_ENTITIES,
	GAME_END,
	LOCK_EXIT,
	PLACE_ITEM_IN_ITEM,
	REMOVE_ENTITIES,
	REPLACE_ENTITY,
	UNLOCK_EXIT
};

ActionEffectType getActionEffectTypeFromString(const std::string& aText);


#endif // !H_ACTION_EFFECT_TYPE
