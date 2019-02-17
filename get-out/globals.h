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


extern bool gIsGameEndRequested;

void consoleLog(const std::string& aMessage);
void consoleLog(char aCharacter);
void outputLog(const char* aFile, int aLine, const char* aFormat, ...);
bool caselessEquals(const std::string& aS1, const std::string& aS2);


#endif // !H_GLOBALS
