#ifndef H_EFFECT_UNLOCK_EXIT
#define H_EFFECT_UNLOCK_EXIT

#include "ActionEffect.h"
class Exit;


class EffectUnlockExit :
	public ActionEffect
{
public:
	EffectUnlockExit(const std::string& aEffectDescription, Exit* aExitToUnlock);
	~EffectUnlockExit();

	// Inherited via ActionEffect
	virtual void doEffect() const override;

private:
	Exit* mExit = nullptr;
};


#endif // !H_EFFECT_UNLOCK_EXIT
