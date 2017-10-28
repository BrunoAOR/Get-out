#ifndef H_INPUT_PARSER
#define H_INPUT_PARSER

#include <vector>
#include <string>
struct Instruction;


class InputParser
{
public:
	InputParser();
	~InputParser();

	Instruction* parse(const std::string& userInput) const;

private:
	std::vector<std::string> parseToVector(const std::string& userInput) const;
	bool checkInputValidity(const std::vector<std::string>& input, Instruction* instruction) const;
};


#endif // !H_INPUT_PARSER
