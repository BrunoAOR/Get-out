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
	INVENTORY,
	GO,
	TAKE,
	DROP,
	INSPECT,
	OPEN,
	USE,
	PUT
};


std::vector<std::string> getActionTypeNames();
ActionType getActionFromString(const std::string& s);
std::string getStringFromAction(ActionType actionType);
std::string getActionPreposition(ActionType actionType);
unsigned int getActionExpectedLength(ActionType actionType);


#endif // !H_ACTION_TYPE
