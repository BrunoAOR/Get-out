#include "EffectGameEnd.h"

#include "globals.h"


EffectGameEnd::EffectGameEnd(const std::string& effectDescription)
	: ActionEffect(effectDescription)
{
}


EffectGameEnd::~EffectGameEnd()
{
}


void EffectGameEnd::doEffect() const
{
	isGameEndRequested = true;
}
