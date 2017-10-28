#ifndef H_DIRECTION
#define H_DIRECTION

#include <vector>
#include <string>


enum class Direction
{
	_UNDEFINED,
	N,
	S,
	E,
	W
};

std::vector<std::string> getDirectionNames();
Direction getDirectionFromString(const std::string& s);
std::string getStringFromDirection(Direction direction);


#endif // !H_DIRECTION
