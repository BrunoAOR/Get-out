#include "EffectLockExit.h"

#include "globals.h"
#include "Exit.h"


EffectLockExit::EffectLockExit(const std::string& aEffectDescription, Exit* aExitToLock)
	: ActionEffect(aEffectDescription), mExit(aExitToLock)
{
	ASSERT(mExit);
}


EffectLockExit::~EffectLockExit()
{
}

void EffectLockExit::doEffect() const
{
	mExit->lock();
}
