#include "InstructionType.h"

#include "globals.h"


InstructionType getInstructionFromString(const std::string& aText)
{
	if (caselessEquals(aText, "ERROR"))		return InstructionType::ERROR;
	if (caselessEquals(aText, "QUIT"))		return InstructionType::QUIT;
	if (caselessEquals(aText, "HELP"))		return InstructionType::HELP;
	if (caselessEquals(aText, "LOOK"))		return InstructionType::LOOK;
	if (caselessEquals(aText, "INVENTORY"))	return InstructionType::INVENTORY;
	if (caselessEquals(aText, "GO"))			return InstructionType::GO;
	if (caselessEquals(aText, "TAKE"))		return InstructionType::TAKE;
	if (caselessEquals(aText, "DROP"))		return InstructionType::DROP;
	if (caselessEquals(aText, "INSPECT"))	return InstructionType::INSPECT;
	if (caselessEquals(aText, "OPEN"))		return InstructionType::OPEN;
	if (caselessEquals(aText, "USE"))		return InstructionType::USE;
	if (caselessEquals(aText, "PUT"))		return InstructionType::PUT;
	return InstructionType::ERROR;
}


std::string getStringFromInstruction(InstructionType aInstructionType)
{
	switch (aInstructionType)
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
	}
	return "";
}


std::string getInstructionPreposition(InstructionType aInstructionType)
{
	switch (aInstructionType)
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
	}
	return "";
}


unsigned int getInstructionExpectedLength(InstructionType aInstructionType)
{
	switch (aInstructionType)
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
	}
	return 0;
}
