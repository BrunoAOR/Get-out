#ifndef H_ACTION_INFO
#define H_ACTION_INFO


#include <string>
#include <vector>
struct ActionEffectInfo;
enum class ActionType;


struct ActionInfo
{
public:
	ActionType mType;
	std::string mDescription;
	bool mShouldDestroy = false;
	int mFirstEntity = -1;
	int mSecondEntity = -1;
	std::vector< ActionEffectInfo > mEffectInfos;
};


#endif // !H_ACTION_INFO

