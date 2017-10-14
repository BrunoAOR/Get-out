#include "Direction.h"

#include "globals.h"


std::vector<std::string> getDirectionNames()
{
	static std::vector<std::string> namesList{ "NORTH", "SOUTH", "EAST", "WEST" };
	return namesList;
}


Direction getDirectionFromString(const std::string & s)
{
	if (caselessEquals(s, "N") || caselessEquals(s, "NORTH")) return Direction::N;
	if (caselessEquals(s, "S") || caselessEquals(s, "SOUTH")) return Direction::S;
	if (caselessEquals(s, "E") || caselessEquals(s, "EAST")) return Direction::E;
	if (caselessEquals(s, "W") || caselessEquals(s, "WEST")) return Direction::W;
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
