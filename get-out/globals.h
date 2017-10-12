#ifndef H_GLOBALS
#define H_GLOBALS

#include <string>
#define OutputLog(format, ...) outputLog(__FILE__, __LINE__, format, __VA_ARGS__);

enum class LoopStatus
{
	InitError,
	UpdateError,
	CloseError,
	Continue,
	Exit
};


void consoleLog(const std::string& message);
void consoleLog(char character);
void outputLog(const char file[], int line, const char* format, ...);
bool caseLessEquals(const std::string& s1, const std::string& s2);


#endif // !H_GLOBALS
