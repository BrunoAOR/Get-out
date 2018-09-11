#ifndef H_ACTION_TYPE
#define H_ACTION_TYPE

#include <string>


enum class ActionType
{
	_UNDEFINED,
	GO,
	TAKE,
	DROP,
	INTERACTABLE_OPEN,
	ITEM_USE,
	ITEM_PUT
};

ActionType getActionTypeFromString(const std::string& text);


#endif // !H_ACTION_TYPE
