#include "InstructionType.h"

#include "globals.h"


std::vector<std::string> getInstructionTypeNames()
{
	static std::vector<std::string> namesList{ "ERROR", "QUIT", "HELP", "LOOK", "INVENTORY", "GO", "TAKE", "DROP", "INSPECT", "OPEN", "USE", "PUT" };
	return namesList;
}


InstructionType getInstructionFromString(const std::string & s)
{
	if (caselessEquals(s, "ERROR"))		return InstructionType::ERROR;
	if (caselessEquals(s, "QUIT"))		return InstructionType::QUIT;
	if (caselessEquals(s, "HELP"))		return InstructionType::HELP;
	if (caselessEquals(s, "LOOK"))		return InstructionType::LOOK;
	if (caselessEquals(s, "INVENTORY"))	return InstructionType::INVENTORY;
	if (caselessEquals(s, "GO"))		return InstructionType::GO;
	if (caselessEquals(s, "TAKE"))		return InstructionType::TAKE;
	if (caselessEquals(s, "DROP"))		return InstructionType::DROP;
	if (caselessEquals(s, "INSPECT"))	return InstructionType::INSPECT;
	if (caselessEquals(s, "OPEN"))		return InstructionType::OPEN;
	if (caselessEquals(s, "USE"))		return InstructionType::USE;
	if (caselessEquals(s, "PUT"))		return InstructionType::PUT;
	return InstructionType::_UNDEFINED;
}


std::string getStringFromInstruction(InstructionType instructionType)
{
	switch (instructionType)
	{
	case InstructionType::ERROR:		return "ERROR";
	case InstructionType::QUIT:			return "QUIT";
	case InstructionType::HELP:			return "HELP";
	case InstructionType::LOOK:			return "LOOK";
	case InstructionType::INVENTORY:	return "INVENTORY";
	case InstructionType::GO:			return "GO";
	case InstructionType::TAKE:			return "TAKE";
	case InstructionType::DROP:			return "DROP";
	case InstructionType::INSPECT:		return "INSPECT";
	case InstructionType::OPEN:			return "OPEN";
	case InstructionType::USE:			return "USE";
	case InstructionType::PUT:			return "PUT";
	default:							return "";
	// default case includes having an _UNDEFINED
	}
}


std::string getInstructionPreposition(InstructionType instructionType)
{
	switch (instructionType)
	{
	case InstructionType::ERROR:		return "";
	case InstructionType::QUIT:			return "";
	case InstructionType::HELP:			return "";
	case InstructionType::LOOK:			return "";
	case InstructionType::INVENTORY:	return "";
	case InstructionType::GO:			return "";
	case InstructionType::TAKE:			return "";
	case InstructionType::DROP:			return "";
	case InstructionType::INSPECT:		return "";
	case InstructionType::OPEN:			return "";
	case InstructionType::USE:			return "ON";
	case InstructionType::PUT:			return "IN";
	default:							return "";
	}
}


unsigned int getInstructionExpectedLength(InstructionType instructionType)
{
	switch (instructionType)
	{
	case InstructionType::ERROR:		return 1;
	case InstructionType::QUIT:			return 1;
	case InstructionType::HELP:			return 1;
	case InstructionType::LOOK:			return 1;
	case InstructionType::INVENTORY:	return 1;
	case InstructionType::GO:			return 2;
	case InstructionType::TAKE:			return 2;
	case InstructionType::DROP:			return 2;
	case InstructionType::INSPECT:		return 2;
	case InstructionType::OPEN:			return 2;
	case InstructionType::USE:			return 4;
	case InstructionType::PUT:			return 4;
	default:							return 0;
	}
}
