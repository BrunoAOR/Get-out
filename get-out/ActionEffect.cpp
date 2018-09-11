#include "ActionEffect.h"


ActionEffect::ActionEffect(const std::string& effectDescription)
	:m_description(effectDescription)
{
}


ActionEffect::~ActionEffect()
{
}


const std::string& ActionEffect::getEffectDescription() const
{
	return m_description;
}
