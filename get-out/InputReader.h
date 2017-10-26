#ifndef H_INPUT_READER
#define H_INPUT_READER

#include <string>


class InputReader
{
public:
	InputReader();
	~InputReader();
	std::string getInput();
	bool getEnter();

private:
	std::string m_input;
	bool m_shouldClear = true;

	bool isAllowed(char c) const;
};


#endif // !H_INPUT_READER
