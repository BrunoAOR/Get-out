#ifndef H_ACTION
#define H_ACTION

#include <string>
#include <vector>
#include "ActionFactory.h"
class Entity;
class ActionEffect;
class ActionFactory;
enum class ActionType;


class Action
{
	friend Action* ActionFactory::createAction(ActionType, const std::string&, const std::vector<ActionEffect*>&, bool, int, int);
	friend void ActionFactory::close();
	friend void ActionFactory::removeAction(Action*);
private:
	Action(ActionFactory* aActionFactory, ActionType aType, const std::string& aDescription, const std::vector<ActionEffect*>& aEffects, bool aShouldDestroy, Entity* aFirstEntity, Entity* aSecondEntity = nullptr);
	~Action();

public:
	ActionType getActionType() const;
	const std::string& getDescription() const;
	const Entity* getFirstEntity() const;
	const Entity* getSecondEntity() const;
	void performAction();

private:
	ActionType mType;
	std::string mDescription;
	std::vector<ActionEffect*> mEffects;
	bool mShouldDestroy;
	Entity* mFirstEntity = nullptr;
	Entity* mSecondEntity = nullptr;
	ActionFactory* mActionFactory = nullptr;
};


#endif // !H_ACTION

