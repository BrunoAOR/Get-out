#ifndef H_EFFECT_LOCK_EXIT
#define H_EFFECT_LOCK_EXIT


#include "ActionEffect.h"
class Exit;


class EffectLockExit :
	public ActionEffect
{
public:
	EffectLockExit(const std::string& aEffectDescription, Exit* aExitToLock);
	~EffectLockExit();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	Exit* mExit = nullptr;
};


#endif // !H_EFFECT_LOCK_EXIT
