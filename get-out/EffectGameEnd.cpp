#include "EffectGameEnd.h"
#include "World.h"


EffectGameEnd::EffectGameEnd(const std::string& effectDescription)
	: ActionEffect(effectDescription)
{
}


EffectGameEnd::~EffectGameEnd()
{
}

void EffectGameEnd::doEffect()
{
	world->requestGameEnd();
}
