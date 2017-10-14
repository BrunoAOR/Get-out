#ifndef H_ACTION_TYPE
#define H_ACTION_TYPE

#include <vector>
#include <string>


enum class ActionType
{
	_UNDEFINED,
	ERROR,
	QUIT,
	HELP,
	LOOK,
	GO,
	TAKE,
	DROP,
	INSPECT,
	OPEN,
	USE,
	PUT
};


enum class ActionParameterType
{
	NONE,
	DIRECTION,
	ITEM,
	INTERACTABLE,
	ITEM_OR_INTERACTABLE
};


std::vector<std::string> getActionTypeNames();
ActionType getActionFromString(const std::string& s);
std::string getStringFromAction(ActionType actionType);
std::string getActionPreposition(ActionType actionType);
unsigned int getActionExpectedLength(ActionType actionType);
ActionParameterType getActionParameterType(ActionType actionType, unsigned int position);


#endif // !H_ACTION_TYPE
