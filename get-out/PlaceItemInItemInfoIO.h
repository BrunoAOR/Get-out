#ifndef H_PLACE_ITEM_IN_ITEM_INFO_IO
#define H_PLACE_ITEM_IN_ITEM_INFO_IO

#include "globals.h"
#include "ActionEffectInfoIO.h"


class PlaceItemInItemInfoIO :
	public ActionEffectInfoIO
{
public:
	PlaceItemInItemInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo)
	{
		ASSERT(getType() == ActionEffectType::PLACE_ITEM_IN_ITEM);
	}

	static const ActionEffectInfo createInfo(const std::string& aDescription, int aItemId, int aContainerId);

	int getItemId() const;
	int getContainerId() const;
};


inline const ActionEffectInfo PlaceItemInItemInfoIO::createInfo(const std::string& aDescription, int aItemId, int aContainerId)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::PLACE_ITEM_IN_ITEM, aDescription);
	addId(lInfo, aItemId);
	addId(lInfo, aContainerId);
	return lInfo;
}


inline int PlaceItemInItemInfoIO::getItemId() const
{
	return getEntityIds()[0];
}


inline int PlaceItemInItemInfoIO::getContainerId() const
{
	return getEntityIds()[1];
}


#endif // !H_PLACE_ITEM_IN_ITEM_INFO_IO
