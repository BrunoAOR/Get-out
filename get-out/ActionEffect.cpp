#include "ActionEffect.h"



ActionEffect::ActionEffect(std::string effectDescription)
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
