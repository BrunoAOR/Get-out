#ifndef H_DIRECTION
#define H_DIRECTION

#include <string>


enum class Direction
{
	_UNDEFINED,
	N,
	S,
	E,
	W
};

Direction getDirectionFromString(const std::string& aText);
std::string getStringFromDirection(Direction aDirection);


#endif // !H_DIRECTION
