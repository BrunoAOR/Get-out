#ifndef H_ACTION_EFFECT
#define H_ACTION_EFFECT

#include <string>


class ActionEffect
{
public:
	ActionEffect(std::string effectDescription);
	~ActionEffect();

	virtual void doEffect() = 0;
	std::string description;
};


#endif // !H_ACTION_EFFECT
