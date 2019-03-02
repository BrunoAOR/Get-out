#ifndef H_ACTION_INFO
#define H_ACTION_INFO


#include <string>
#include <vector>
struct ActionEffectInfo;
enum class ActionType;


struct ActionInfo
{
public:
	using const_iterator = std::vector< ActionEffectInfo >::const_iterator;

	const_iterator actionEffectInfosBegin() const { return mEffectInfos.begin(); }
	const_iterator actionEffectInfosEnd() const { return mEffectInfos.end(); }

	void addActionEffectInfo(ActionEffectInfo aInfo) { mEffectInfos.push_back(aInfo); }

public:
	ActionType mType;
	std::string mDescription;
	bool mShouldDestroy = false;
	int mFirstEntity = -1;
	int mSecondEntity = -1;

private:
	std::vector< ActionEffectInfo > mEffectInfos;
};


#endif // !H_ACTION_INFO

