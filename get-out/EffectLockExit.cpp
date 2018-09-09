#include "EffectLockExit.h"

#include <assert.h>
#include "Exit.h"


EffectLockExit::EffectLockExit(const std::string& effectDescription, Exit* exitToLock)
	: ActionEffect(effectDescription), m_exit(exitToLock)
{
	assert(m_exit);
}


EffectLockExit::~EffectLockExit()
{
}

void EffectLockExit::doEffect() const
{
	m_exit->lock();
}
