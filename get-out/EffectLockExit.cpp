#include "EffectLockExit.h"

#include <assert.h>
#include "Exit.h"


EffectLockExit::EffectLockExit(const std::string& aEffectDescription, Exit* aExitToLock)
	: ActionEffect(aEffectDescription), mExit(aExitToLock)
{
	assert(mExit);
}


EffectLockExit::~EffectLockExit()
{
}

void EffectLockExit::doEffect() const
{
	mExit->lock();
}
