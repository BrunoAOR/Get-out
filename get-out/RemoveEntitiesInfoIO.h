#ifndef H_REMOVE_ENTITIES_INFO_IO
#define H_REMOVE_ENTITIES_INFO_IO

#include "globals.h"
#include "ActionEffectInfoIO.h"


class RemoveEntitiesInfoIO :
	public ActionEffectInfoIO
{
public:
	RemoveEntitiesInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo)
	{
		ASSERT(getType() == ActionEffectType::REMOVE_ENTITIES);
	}

	template< typename input_iterator >
	static const ActionEffectInfo createInfo(const std::string& aDescription, input_iterator aEntitiesToRemoveBeginIt, input_iterator aEntitiesToRemoveEndIt);

	ActionEffectInfo::const_iterator entitiesToRemoveBegin() const;
	ActionEffectInfo::const_iterator entitiesToRemoveEnd() const;
};

template<typename input_iterator>
inline const ActionEffectInfo RemoveEntitiesInfoIO::createInfo(const std::string& aDescription, input_iterator aEntitiesToRemoveBeginIt, input_iterator aEntitiesToRemoveEndIt)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::REMOVE_ENTITIES, aDescription);
	addIds(lInfo, aEntitiesToRemoveBeginIt, aEntitiesToRemoveEndIt);
	return lInfo;
}


inline ActionEffectInfo::const_iterator RemoveEntitiesInfoIO::entitiesToRemoveBegin() const
{
	return ++getEntityIds().begin();
}


inline ActionEffectInfo::const_iterator RemoveEntitiesInfoIO::entitiesToRemoveEnd() const
{
	return getEntityIds().end();
}


#endif // !H_REMOVE_ENTITIES_INFO_IO
