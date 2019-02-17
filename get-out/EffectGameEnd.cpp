#include "EffectGameEnd.h"

#include "globals.h"


EffectGameEnd::EffectGameEnd(const std::string& aEffectDescription)
	: ActionEffect(aEffectDescription)
{
}


EffectGameEnd::~EffectGameEnd()
{
}


void EffectGameEnd::doEffect() const
{
	gIsGameEndRequested = true;
}
