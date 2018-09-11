#include "Direction.h"

#include "globals.h"


Direction getDirectionFromString(const std::string& text)
{
	if (caselessEquals(text, "N") || caselessEquals(text, "NORTH")) return Direction::N;
	if (caselessEquals(text, "S") || caselessEquals(text, "SOUTH")) return Direction::S;
	if (caselessEquals(text, "E") || caselessEquals(text, "EAST")) return Direction::E;
	if (caselessEquals(text, "W") || caselessEquals(text, "WEST")) return Direction::W;
	return Direction::_UNDEFINED;
}


std::string getStringFromDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::N:	return "NORTH";
	case Direction::S:	return "SOUTH";
	case Direction::E:	return "EAST";
	case Direction::W:	return "WEST";
	default:			return "";
	}
}
