#ifndef H_WORLD
#define H_WORLD

#include <vector>
#include <string>
#include <assert.h>

enum class LoopStatus;
enum class ActionType;
class Player;
class InputParser;
class Entity;
class Action;
class ActionEffect;


class World
{
public:
	World();
	~World();

	LoopStatus init();
	LoopStatus update(const std::string& userInput);
	LoopStatus close();

	Entity* getEntity(const std::string& m_name);
	Action* getAction(ActionType actionType, const std::string& firstEntityName, const std::string& secondEntityName = "");
	void removeAction(Action* action);

private:
	Player* player = nullptr;
	InputParser* m_inputParser = nullptr;
	std::vector<Entity*> m_entities;
	std::vector<Action*> m_actions;
	
	void logWelcomeMessage();
	void logHelpMessage();

	template <typename T>
	void deleteCollection(std::vector<T>& collection);
};

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
