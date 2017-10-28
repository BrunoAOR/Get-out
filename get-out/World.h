#ifndef H_WORLD
#define H_WORLD

#include <vector>
#include <string>
#include <assert.h>
#include "globals.h"
class Player;
class InputParser;
class EntityFactory;
class Entity;
class ActionFactory;
class Action;
class ActionEffect;
enum class LoopStatus;
enum class ActionType;


class World
{
public:
	World();
	~World();
	World(const World& source) = delete;
	World& operator=(const World& source) = delete;

	LoopStatus init();
	LoopStatus update(const std::string& userInput);
	LoopStatus close();

	Action* getAction(ActionType actionType, const Entity* firstEntity, const Entity* secondEntity = nullptr) const;
	void removeAction(Action* action) const;

	void requestGameEnd();

private:
	Player* player = nullptr;
	InputParser* m_inputParser = nullptr;
	EntityFactory* m_entityFactory = nullptr;
	ActionFactory* m_actionFactory = nullptr;
	LoopStatus m_loopStatus = LoopStatus::CONTINUE;

	void logHelpMessage();
};

// TODO: Remove the global world variable and pass a reference to whoever requires it
extern World* world;


#endif // !H_WORLD
