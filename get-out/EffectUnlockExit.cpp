#include "EffectUnlockExit.h"

#include "globals.h"
#include "Exit.h"


EffectUnlockExit::EffectUnlockExit(const std::string& aEffectDescription, Exit* aExitToUnlock)
	: ActionEffect(aEffectDescription), mExit(aExitToUnlock)
{
	ASSERT(mExit);
}


EffectUnlockExit::~EffectUnlockExit()
{
}


void EffectUnlockExit::doEffect() const
{
	mExit->unlock();
}
