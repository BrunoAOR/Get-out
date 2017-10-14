#ifndef H_INPUT_PARSER
#define H_INPUT_PARSER

#include <vector>
#include <string>
struct Instruction;
struct NamesInfo;
enum class ActionParameterType;


class InputParser
{
public:
	InputParser();
	~InputParser();

	Instruction* parse(const std::string& userInput, const NamesInfo& namesInfo) const;

private:
	std::vector<std::string> parseToVector(const std::string& userInput) const;
	bool checkInputValidity(const std::vector<std::string>& input, const NamesInfo& namesInfo, Instruction* instruction) const;
	bool populateInstructionUnion(const std::vector<std::string>& validatedInput, Instruction* instruction) const;
	bool verifyActionParameterType(ActionParameterType actionParameterType, const std::string& parameter, const NamesInfo& namesInfo, Instruction* instruction) const;
	bool isCaselessStringInVector(const std::vector<std::string>& vector, const std::string string) const;
};


#endif // !H_INPUT_PARSER
