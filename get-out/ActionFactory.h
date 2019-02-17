#ifndef H_ACTION_FACTORY
#define H_ACTION_FACTORY

#include <vector>
#include <list>
#include <string>
class Entity;
class Action;
class EntityFactory;
class ActionEffect;
enum class ActionType;


class ActionFactory
{
public:
	ActionFactory(EntityFactory* aEntityFactory);
	~ActionFactory();
	ActionFactory(const ActionFactory& aSource) = delete;
	ActionFactory& operator=(const ActionFactory& aSource) = delete;

	void close();

	Action* createAction(ActionType aType, const std::string& aDescription, const std::vector<ActionEffect*>& aEffects, bool aShouldDestroy, int aFirstEntityId, int aSecondEntityId = -1);
	
	Action* getAction(ActionType aActionType, const Entity* aFirstEntity, const Entity* aSecondEntity = nullptr) const;

	void removeAction(Action* aAction);

private:
	std::list<Action*> mActions;
	EntityFactory* mEntityFactory = nullptr;
};


#endif // !H_ACTION_FACTORY
