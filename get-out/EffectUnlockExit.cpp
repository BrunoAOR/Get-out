#include "EffectUnlockExit.h"
#include "Exit.h"
#include <assert.h>


EffectUnlockExit::EffectUnlockExit(const std::string& effectDescription, Exit * exitToUnlock)
	: ActionEffect(effectDescription), m_exit(exitToUnlock)
{
	assert(m_exit);
}

EffectUnlockExit::~EffectUnlockExit()
{
}

void EffectUnlockExit::doEffect()
{
	m_exit->unlock();
}
