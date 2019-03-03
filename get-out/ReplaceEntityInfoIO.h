#ifndef H_REPLACE_ENTITY_INFO_IO
#define H_REPLACE_ENTITY_INFO_IO

#include "globals.h"
#include "ActionEffectInfoIO.h"


class ReplceEntityInfoIO :
	public ActionEffectInfoIO
{
public:
	ReplceEntityInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo)
	{
		ASSERT(getType() == ActionEffectType::REPLACE_ENTITY);
	}

	static const ActionEffectInfo createInfo(const std::string& aDescription, int aEntityToRemoveId, int aEntityToAddId);

	int getEntityToRemoveId() const;
	int getEntityToAddId() const;
};


inline const ActionEffectInfo ReplceEntityInfoIO::createInfo(const std::string& aDescription, int aEntityToRemoveId, int aEntityToAddId)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::REPLACE_ENTITY, aDescription);
	addId(lInfo, aEntityToRemoveId);
	addId(lInfo, aEntityToAddId);
	return lInfo;
}


inline int ReplceEntityInfoIO::getEntityToRemoveId() const
{
	return getEntityIds()[0];
}


inline int ReplceEntityInfoIO::getEntityToAddId() const
{
	return getEntityIds()[1];
}


#endif // !H_REPLACE_ENTITY_INFO_IO
