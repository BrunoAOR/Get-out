#ifndef H_ACTION_EFFECT_INFO
#define H_ACTION_EFFECT_INFO

#include <string>
#include <vector>
enum class ActionEffectType;


struct ActionEffectInfo
{
public:
	friend class ActionEffectInfoIO;
	using const_iterator = std::vector< int >::const_iterator;
	
private:
	ActionEffectInfo(ActionEffectType aType, const std::string& aDescription)
		: mType(aType), mDescription(aDescription) { ; }

	void addId(int aId);
	template< typename input_iterator >
	void addIds(input_iterator aBeginIt, input_iterator aEndIt);

private:
	ActionEffectType mType;
	std::string mDescription;
	std::vector< int > mEntityIds;
};

inline void ActionEffectInfo::addId(int aId)
{
	mEntityIds.push_back(aId);
}

template<typename input_iterator>
inline void ActionEffectInfo::addIds(input_iterator aBeginIt, input_iterator aEndIt)
{
	mEntityIds.insert(mEntityIds.end(), aBeginIt, aEndIt);
}


#endif // !H_ACTION_EFFECT_INFO
