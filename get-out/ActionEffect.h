#ifndef H_ACTION_EFFECT
#define H_ACTION_EFFECT

#include <string>


class ActionEffect
{
public:
	ActionEffect(std::string effectDescription);
	~ActionEffect();

	std::string getEffectDescription();

	virtual void doEffect() = 0;

private:
	std::string m_description;
};


#endif // !H_ACTION_EFFECT
