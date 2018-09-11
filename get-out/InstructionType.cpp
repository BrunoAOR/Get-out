#include "InstructionType.h"

#include "globals.h"


InstructionType getInstructionFromString(const std::string& text)
{
	if (caselessEquals(text, "ERROR"))		return InstructionType::ERROR;
	if (caselessEquals(text, "QUIT"))		return InstructionType::QUIT;
	if (caselessEquals(text, "HELP"))		return InstructionType::HELP;
	if (caselessEquals(text, "LOOK"))		return InstructionType::LOOK;
	if (caselessEquals(text, "INVENTORY"))	return InstructionType::INVENTORY;
	if (caselessEquals(text, "GO"))			return InstructionType::GO;
	if (caselessEquals(text, "TAKE"))		return InstructionType::TAKE;
	if (caselessEquals(text, "DROP"))		return InstructionType::DROP;
	if (caselessEquals(text, "INSPECT"))	return InstructionType::INSPECT;
	if (caselessEquals(text, "OPEN"))		return InstructionType::OPEN;
	if (caselessEquals(text, "USE"))		return InstructionType::USE;
	if (caselessEquals(text, "PUT"))		return InstructionType::PUT;
	return InstructionType::ERROR;
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
	}
	return "";
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
	}
	return "";
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
	}
	return 0;
}
