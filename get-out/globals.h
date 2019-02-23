#ifndef H_GLOBALS
#define H_GLOBALS

constexpr char* CONFIG_FILE_PATH = "gameConfig.json";

#undef OUTPUT_LOG
#undef ASSERT
#undef ASSERT_MSG

#ifdef NDEBUG

#define OUTPUT_LOG(format, ...) ((void)0)
#define ASSERT(expression) ((void)0)
#define ASSERT_MSG(expression, message, ...) ((void)0)

#else

#define OUTPUT_LOG(format, ...) outputLog(__FILE__, __LINE__, format, __VA_ARGS__)
#define ASSERT(expression) assertLog(expression, #expression, __FILE__, __LINE__)
#define ASSERT_MSG(expression, message, ...) assertMsg(expression, message, #expression, __FILE__, __LINE__, __VA_ARGS__)

#endif

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
void assertLog(bool expression, const char* aExpressionText, const char* aFile, int aLine);
void assertMsg(bool expression, const char* aFormat, const char* aExpressionText, const char* aFile, int aLine, ...);
bool caselessEquals(const std::string& aS1, const std::string& aS2);


#endif // !H_GLOBALS
