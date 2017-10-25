#include "ActionEffect.h"



ActionEffect::ActionEffect(const std::string& effectDescription)
	:m_description(effectDescription)
{
}


ActionEffect::~ActionEffect()
{
}


std::string ActionEffect::getEffectDescription()
{
	return m_description;
}
