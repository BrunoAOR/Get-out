#ifndef H_GAME_END_INFO_IO
#define H_GAME_END_INFO_IO

#include "globals.h"
#include "ActionEffectInfoIO.h"


class GameEndInfoIO :
	public ActionEffectInfoIO
{
public:
	GameEndInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo)
	{
		ASSERT(getType() == ActionEffectType::GAME_END);
	}

	static const ActionEffectInfo createInfo(const std::string& aDescription);
};


inline const ActionEffectInfo GameEndInfoIO::createInfo(const std::string& aDescription)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::GAME_END, aDescription);
	return lInfo;
}


#endif // !H_GAME_END_INFO_IO
