#ifndef H_GLOBALS
#define H_GLOBALS

#define OutputLog(format, ...) outputLog(__FILE__, __LINE__, format, __VA_ARGS__)
#define CONFIG_FILE_PATH "gameConfig.json"

#include <string>


enum class LoopStatus
{
	INIT_ERROR,
	UPDATE_ERROR,
	CLOSE_ERROR,
	CONTINUE,
	EXIT
};


extern bool isGameEndRequested;

void consoleLog(const std::string& message);
void consoleLog(char character);
void outputLog(const char* file, int line, const char* format, ...);
bool caselessEquals(const std::string& s1, const std::string& s2);


#endif // !H_GLOBALS
