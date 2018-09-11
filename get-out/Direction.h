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

Direction getDirectionFromString(const std::string& text);
std::string getStringFromDirection(Direction direction);


#endif // !H_DIRECTION
