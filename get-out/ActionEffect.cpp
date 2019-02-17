#include "ActionEffect.h"


ActionEffect::ActionEffect(const std::string& aEffectDescription)
	:mDescription(aEffectDescription)
{
}


ActionEffect::~ActionEffect()
{
}


const std::string& ActionEffect::getEffectDescription() const
{
	return mDescription;
}
