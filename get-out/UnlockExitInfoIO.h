#ifndef H_UNLOCK_EXIT_INFO_IO
#define H_UNLOCK_EXIT_INFO_IO

#include "globals.h"
#include "ActionEffectInfoIO.h"


class UnlockExitInfoIO :
	public ActionEffectInfoIO
{
public:
	UnlockExitInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo)
	{
		ASSERT(getType() == ActionEffectType::UNLOCK_EXIT);
	}

	static const ActionEffectInfo createInfo(const std::string& aDescription, int aExitToUnlockId);

	int getExitToUnlockId() const;
};


inline const ActionEffectInfo UnlockExitInfoIO::createInfo(const std::string& aDescription, int aExitToUnlockId)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::UNLOCK_EXIT, aDescription);
	addId(lInfo, aExitToUnlockId);
	return lInfo;
}


inline int UnlockExitInfoIO::getExitToUnlockId() const
{
	return getEntityIds()[0];
}


#endif // !H_UNLOCK_EXIT_INFO_IO
