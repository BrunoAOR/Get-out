#ifndef H_ADD_ENTITIES_INFO_IO
#define H_ADD_ENTITIES_INFO_IO

#include "ActionEffectInfoIO.h"


class AddEntitiesInfoIO :
	public ActionEffectInfoIO
{
public:
	AddEntitiesInfoIO(const ActionEffectInfo& aInfo) : ActionEffectInfoIO(aInfo) { ; }

	template< typename input_iterator >
	static const ActionEffectInfo createInfo(const std::string& aDescription, int aTargetRoomId, input_iterator aBeginIt, input_iterator aEndIt);

	ActionEffectInfo::const_iterator entitiesToAddBegin() const;
	ActionEffectInfo::const_iterator entitiesToAddEnd() const;
	int getTargetRoomId() const;
};

template<typename input_iterator>
inline const ActionEffectInfo AddEntitiesInfoIO::createInfo(const std::string & aDescription, int aTargetRoomId, input_iterator aBeginIt, input_iterator aEndIt)
{
	ActionEffectInfo lInfo = ActionEffectInfoIO::createInfo(ActionEffectType::ADD_ENTITIES, aDescription);
	addId(lInfo, aTargetRoomId);
	addIds(lInfo, aBeginIt, aEndIt);
	return lInfo;
}


inline ActionEffectInfo::const_iterator AddEntitiesInfoIO::entitiesToAddBegin() const
{
	return ++getEntityIds().begin();
}


inline ActionEffectInfo::const_iterator AddEntitiesInfoIO::entitiesToAddEnd() const
{
	return getEntityIds().end();
}


inline int AddEntitiesInfoIO::getTargetRoomId() const
{
	return getEntityIds()[0];
}


#endif // !H_ADD_ENTITIES_INFO_IO
