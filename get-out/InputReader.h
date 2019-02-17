#ifndef H_INPUT_READER
#define H_INPUT_READER

#include <string>


class InputReader
{
public:
	InputReader();
	~InputReader();
	std::string getInput();
	bool getEnter() const;

private:
	std::string mInput;
	bool mShouldClear = true;

	void handleInvalidKeys(char aKey) const;
	bool isAllowed(char aChar) const;
};


#endif // !H_INPUT_READER
