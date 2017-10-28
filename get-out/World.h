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

private:
	Player* player = nullptr;
	InputParser* m_inputParser = nullptr;
	EntityFactory* m_entityFactory = nullptr;
	ActionFactory* m_actionFactory = nullptr;
	LoopStatus m_loopStatus = LoopStatus::CONTINUE;

	void logHelpMessage();
};


#endif // !H_WORLD
