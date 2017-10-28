#ifndef H_EFFECT_GAME_END
#define H_EFFECT_GAME_END

#include "ActionEffect.h"


class EffectGameEnd :
	public ActionEffect
{
public:
	EffectGameEnd(const std::string& effectDescription);
	~EffectGameEnd();

	// Inherited via ActionEffect
	virtual void doEffect() const override;
};


#endif // !H_EFFECT_GAME_END
