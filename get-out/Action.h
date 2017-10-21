#ifndef H_ACTION
#define H_ACTION

#include <string>
#include <vector>
enum class ActionType;
class Entity;
class ActionEffect;


class Action
{
public:
	Action(ActionType type, std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Entity* firstEntity, Entity* secondEntity);
	// TODO: Consider not having this as an abstract base class, but as the direct-use class if not of the three children have anything special
	virtual ~Action() = 0;

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
};


#endif // !H_ACTION

