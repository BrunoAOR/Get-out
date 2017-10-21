#ifndef H_INSTRUCTION
#define H_INSTRUCTION

#include <string>
enum class ActionType;


struct Instruction
{
public:
	ActionType actionType;
	std::string errorDescription;
	std::string param1;
	std::string param2;
};


#endif // !H_INSTRUCTION
