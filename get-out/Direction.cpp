#include "Direction.h"

#include "globals.h"


Direction getDirectionFromString(const std::string& aText)
{
	if (caselessEquals(aText, "N") || caselessEquals(aText, "NORTH")) return Direction::N;
	if (caselessEquals(aText, "S") || caselessEquals(aText, "SOUTH")) return Direction::S;
	if (caselessEquals(aText, "E") || caselessEquals(aText, "EAST")) return Direction::E;
	if (caselessEquals(aText, "W") || caselessEquals(aText, "WEST")) return Direction::W;
	return Direction::_UNDEFINED;
}


std::string getStringFromDirection(Direction aDirection)
{
	switch (aDirection)
	{
	case Direction::N:	return "NORTH";
	case Direction::S:	return "SOUTH";
	case Direction::E:	return "EAST";
	case Direction::W:	return "WEST";
	default:			return "";
	}
}
