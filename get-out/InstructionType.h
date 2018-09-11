#ifndef H_INSTRUCTION_TYPE
#define H_INSTRUCTION_TYPE

#include <string>


enum class InstructionType
{
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


InstructionType getInstructionFromString(const std::string& text);
std::string getStringFromInstruction(InstructionType instructionType);
std::string getInstructionPreposition(InstructionType instructionType);
unsigned int getInstructionExpectedLength(InstructionType instructionType);


#endif // !H_INSTRUCTION_TYPE
