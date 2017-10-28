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
	friend Action* ActionFactory::createAction(ActionType, const std::string&, std::vector<ActionEffect*>, bool, int, int);
	friend void ActionFactory::close();
	friend void ActionFactory::removeAction(Action* action);
private:
	Action(ActionFactory* actionFactory, ActionType type, const std::string& description, std::vector<ActionEffect*> effects, bool shouldDestroy, Entity* firstEntity, Entity* secondEntity = nullptr);
	~Action();

public:
	ActionType getActionType() const;
	std::string getDescription() const;
	const Entity* getFirstEntity() const;
	const Entity* getSecondEntity() const;
	void performAction();

private:
	ActionType m_type;
	std::string m_description;
	std::vector<ActionEffect*> m_effects;
	bool m_shouldDestroy;
	Entity* m_firstEntity = nullptr;
	Entity* m_secondEntity = nullptr;
	ActionFactory* m_actionFactory = nullptr;
};


#endif // !H_ACTION

