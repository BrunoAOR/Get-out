#ifndef H_INPUT_READER
#define H_INPUT_READER

#include <string>


class InputReader
{
public:
	InputReader();
	~InputReader();
	std::string getInput();

private:
	std::string input;
	bool shouldClear = false;

	bool isAllowed(char c) const;
};


#endif // !H_INPUT_READER
