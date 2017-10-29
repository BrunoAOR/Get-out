#ifndef H_EFFECT_LOCK_EXIT
#define H_EFFECT_LOCK_EXIT


#include "ActionEffect.h"
class Exit;


class EffectLockExit :
	public ActionEffect
{
public:
	EffectLockExit(const std::string& effectDescription, Exit* exitToLock);
	~EffectLockExit();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	Exit* m_exit;
};


#endif // !H_EFFECT_LOCK_EXIT
