#ifndef H_INSTRUCTION_TYPE
#define H_INSTRUCTION_TYPE

#include <vector>
#include <string>


enum class InstructionType
{
	_UNDEFINED,
	ERROR,
	QUIT,
	HELP,
	LOOK,
	INVENTORY,
	GO,
	TAKE,
	DROP,
	INSPECT,
	OPEN,
	USE,
	PUT
};


std::vector<std::string> getInstructionTypeNames();
InstructionType getInstructionFromString(const std::string& s);
std::string getStringFromInstruction(InstructionType instructionType);
std::string getInstructionPreposition(InstructionType instructionType);
unsigned int getInstructionExpectedLength(InstructionType instructionType);


#endif // !H_INSTRUCTION_TYPE
