#include "EffectUnlockExit.h"

#include <assert.h>
#include "Exit.h"


EffectUnlockExit::EffectUnlockExit(const std::string& aEffectDescription, Exit* aExitToUnlock)
	: ActionEffect(aEffectDescription), mExit(aExitToUnlock)
{
	assert(mExit);
}


EffectUnlockExit::~EffectUnlockExit()
{
}


void EffectUnlockExit::doEffect() const
{
	mExit->unlock();
}
