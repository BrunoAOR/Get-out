#ifndef H_ACTION_EFFECT
#define H_ACTION_EFFECT

#include <string>


class ActionEffect
{
public:
	ActionEffect(const std::string& effectDescription);
	virtual ~ActionEffect();

	const std::string& getEffectDescription() const;
	virtual void doEffect() const = 0;

private:
	std::string m_description;
};


#endif // !H_ACTION_EFFECT
