#ifndef H_ACTION_EFFECT_INFO_IO
#define H_ACTION_EFFECT_INFO_IO

#include "ActionEffectInfo.h"
#include "ActionEffectType.h"


class ActionEffectInfoIO
{
public:
	ActionEffectInfoIO& operator= (const ActionEffectInfoIO& aSource) = delete;

	static ActionEffectType getType(const ActionEffectInfo& aInfo) { return aInfo.mType; }

	ActionEffectType getType() const;
	const std::string& getDescription() const;

protected:
	ActionEffectInfoIO(const ActionEffectInfo& aInfo) : mEffectInfo(aInfo) { ; }

protected:
	const std::vector< int >& getEntityIds() const;

	static ActionEffectInfo createInfo(ActionEffectType aType, const std::string& aDescription);
	static void addId(ActionEffectInfo& aInOutInfo, int aId);
	template< typename input_iterator >
	static void addIds(ActionEffectInfo& aInOutInfo, input_iterator aBeginIt, input_iterator aEndIt);

private:
	const ActionEffectInfo& mEffectInfo;
};

template<typename input_iterator>
inline void ActionEffectInfoIO::addIds(ActionEffectInfo& aInOutInfo, input_iterator aBeginIt, input_iterator aEndIt)
{
	aInOutInfo.addIds(aBeginIt, aEndIt);
}

inline ActionEffectInfo ActionEffectInfoIO::createInfo(ActionEffectType aType, const std::string & aDescription)
{
	return ActionEffectInfo(aType, aDescription);
}


inline void ActionEffectInfoIO::addId(ActionEffectInfo & aInOutInfo, int aId)
{
	aInOutInfo.addId(aId);
}


inline ActionEffectType ActionEffectInfoIO::getType() const
{
	return mEffectInfo.mType;
}


inline const std::string & ActionEffectInfoIO::getDescription() const
{
	return mEffectInfo.mDescription;
}


inline const std::vector<int>& ActionEffectInfoIO::getEntityIds() const
{
	return mEffectInfo.mEntityIds;
}


#endif // !H_ACTION_EFFECT_INFO_IO
