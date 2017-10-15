#include "ActionType.h"

#include "globals.h"


std::vector<std::string> getActionTypeNames()
{
	static std::vector<std::string> namesList{ "ERROR", "QUIT", "HELP", "LOOK", "INVENTORY", "GO", "TAKE", "DROP", "INSPECT", "OPEN", "USE", "PUT" };
	return namesList;
}


ActionType getActionFromString(const std::string & s)
{
	if (caselessEquals(s, "ERROR"))		return ActionType::ERROR;
	if (caselessEquals(s, "QUIT"))		return ActionType::QUIT;
	if (caselessEquals(s, "HELP"))		return ActionType::HELP;
	if (caselessEquals(s, "LOOK"))		return ActionType::LOOK;
	if (caselessEquals(s, "INVENTORY"))	return ActionType::INVENTORY;
	if (caselessEquals(s, "GO"))		return ActionType::GO;
	if (caselessEquals(s, "TAKE"))		return ActionType::TAKE;
	if (caselessEquals(s, "DROP"))		return ActionType::DROP;
	if (caselessEquals(s, "INSPECT"))	return ActionType::INSPECT;
	if (caselessEquals(s, "OPEN"))		return ActionType::OPEN;
	if (caselessEquals(s, "USE"))		return ActionType::USE;
	if (caselessEquals(s, "PUT"))		return ActionType::PUT;
	return ActionType::_UNDEFINED;
}


std::string getStringFromAction(ActionType actionType)
{
	switch (actionType)
	{
	case ActionType::ERROR:		return "ERROR";
	case ActionType::QUIT:		return "QUIT";
	case ActionType::HELP:		return "HELP";
	case ActionType::LOOK:		return "LOOK";
	case ActionType::INVENTORY:	return "INVENTORY";
	case ActionType::GO:		return "GO";
	case ActionType::TAKE:		return "TAKE";
	case ActionType::DROP:		return "DROP";
	case ActionType::INSPECT:	return "INSPECT";
	case ActionType::OPEN:		return "OPEN";
	case ActionType::USE:		return "USE";
	case ActionType::PUT:		return "PUT";
	default:					return "";
	// default case includes having an _UNDEFINED
	}
}


std::string getActionPreposition(ActionType actionType)
{
	switch (actionType)
	{
	case ActionType::ERROR:		return "";
	case ActionType::QUIT:		return "";
	case ActionType::HELP:		return "";
	case ActionType::LOOK:		return "";
	case ActionType::INVENTORY:	return "";
	case ActionType::GO:		return "";
	case ActionType::TAKE:		return "";
	case ActionType::DROP:		return "";
	case ActionType::INSPECT:	return "";
	case ActionType::OPEN:		return "";
	case ActionType::USE:		return "ON";
	case ActionType::PUT:		return "IN";
	default:					return "";
	}
}


unsigned int getActionExpectedLength(ActionType actionType)
{
	switch (actionType)
	{
	case ActionType::ERROR:		return 1;
	case ActionType::QUIT:		return 1;
	case ActionType::HELP:		return 1;
	case ActionType::LOOK:		return 1;
	case ActionType::INVENTORY:	return 1;
	case ActionType::GO:		return 2;
	case ActionType::TAKE:		return 2;
	case ActionType::DROP:		return 2;
	case ActionType::INSPECT:	return 2;
	case ActionType::OPEN:		return 2;
	case ActionType::USE:		return 4;
	case ActionType::PUT:		return 4;
	default:					return 0;
	}
}


ActionParameterType getActionParameterType(ActionType actionType, unsigned int position)
{
	
	switch (actionType)
	{
	case ActionType::ERROR:
	case ActionType::QUIT:
	case ActionType::HELP:
	case ActionType::LOOK:
	case ActionType::INVENTORY:
		return ActionParameterType::NONE;
	case ActionType::GO:
		if (position == 1)
		{
			return ActionParameterType::DIRECTION;
		}
		break;
	case ActionType::TAKE:
		if (position == 1)
		{
			return ActionParameterType::ITEM;
		}
		break;
	case ActionType::DROP:
		if (position == 1)
		{
			return ActionParameterType::ITEM;
		}
		break;
	case ActionType::INSPECT:
		if (position == 1)
		{
			return ActionParameterType::ITEM_OR_INTERACTABLE;
		}
	case ActionType::OPEN:
		if (position == 1)
		{
			return ActionParameterType::INTERACTABLE;
		}
		break;
	case ActionType::USE:
		if (position == 1)
		{
			return ActionParameterType::ITEM;
		}
		else if (position == 3)
		{
			return ActionParameterType::INTERACTABLE;
		}
		break;
	case ActionType::PUT:
		if (position == 1 || position == 3)
		{
			return ActionParameterType::ITEM;
		}
		break;
	default:
		return ActionParameterType::NONE;
	}

	return ActionParameterType::NONE;
}
