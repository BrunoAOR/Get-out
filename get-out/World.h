#ifndef H_WORLD
#define H_WORLD

#include <vector>
#include <string>
#include <assert.h>

enum class LoopStatus;
enum class ActionType;
class Player;
class InputParser;
class EntityFactory;
class Entity;
class ActionFactory;
class Action;
class ActionEffect;


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

	Entity* getEntity(const std::string& name);
	Action* getAction(ActionType actionType, const std::string& firstEntityName, const std::string& secondEntityName = "");
	void removeAction(Action* action);

private:
	Player* player = nullptr;
	InputParser* m_inputParser = nullptr;
	EntityFactory* m_entityFactory = nullptr;
	ActionFactory* m_actionFactory = nullptr;
	
	void logWelcomeMessage();
	void logHelpMessage();

	template <typename T>
	void deleteCollection(std::vector<T>& collection);
};

// TODO: Remove the global world variable and pass a reference to whoever requires it
extern World* world;

template<typename T>
void World::deleteCollection(std::vector<T>& collection)
{
	assert(std::is_pointer<T>::value);
	for (T element : collection)
	{
		delete element;
	}
	collection.clear();
}


#endif // !H_WORLD
