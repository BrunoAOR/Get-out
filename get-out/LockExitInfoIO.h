#ifndef H_LOCK_EXIT_INFO_IO
#define H_LOCK_EXIT_INFO_IO

#include "globals.h"
#include "ActionEffectInfoIO.h"


class LockExitInfoIO :
	public ActionEffectInfoIO
{
public:
	LockExitInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo)
	{
		ASSERT(getType() == ActionEffectType::LOCK_EXIT);
	}

	static const ActionEffectInfo createInfo(const std::string& aDescription, int aExitToLockId);

	int getExitToLockId() const;
};


inline const ActionEffectInfo LockExitInfoIO::createInfo(const std::string& aDescription, int aExitToLockId)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::LOCK_EXIT, aDescription);
	addId(lInfo, aExitToLockId);
	return lInfo;
}


inline int LockExitInfoIO::getExitToLockId() const
{
	return getEntityIds()[0];
}


#endif // !H_LOCK_EXIT_INFO_IO
