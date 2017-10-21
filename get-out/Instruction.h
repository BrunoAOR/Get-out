#ifndef H_INSTRUCTION
#define H_INSTRUCTION

#include <string>
enum class InstructionType;


struct Instruction
{
public:
	InstructionType instructionType;
	std::string errorDescription;
	std::string param1;
	std::string param2;
};


#endif // !H_INSTRUCTION
