#include "EffectUnlockExit.h"

#include <assert.h>
#include "Exit.h"


EffectUnlockExit::EffectUnlockExit(const std::string& effectDescription, Exit* exitToUnlock)
	: ActionEffect(effectDescription), m_exit(exitToUnlock)
{
	assert(m_exit);
}


EffectUnlockExit::~EffectUnlockExit()
{
}


void EffectUnlockExit::doEffect() const
{
	m_exit->unlock();
}
