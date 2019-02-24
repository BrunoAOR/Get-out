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

	Instruction parse(const std::string& aUserInput) const;

private:
	std::vector< std::string > parseToVector(const std::string& aUserInput) const;
	Instruction parseToInstruction(const std::vector< std::string >& aInput) const;
};


#endif // !H_INPUT_PARSER
