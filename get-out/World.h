#ifndef H_WORLD
#define H_WORLD

#include <vector>
#include <string>

enum class LoopStatus;
class Player;
class InputParser;
class Entity;
struct InteractableOpen;
struct ItemUse;
struct ItemPut;


class World
{
public:
	World();
	~World();

	LoopStatus init();
	LoopStatus update(const std::string& userInput);
	LoopStatus close();

	Entity* getEntity(const std::string& m_name);
	InteractableOpen* getInteractableOpen(const std::string& interactableName);
	ItemUse* getItemUse(const std::string& itemName, const std::string& interactableName);
	ItemPut* getItemPut(const std::string& itemName, const std::string& containerItemName);

private:
	Player* player = nullptr;
	InputParser* m_inputParser = nullptr;
	std::vector<Entity*> m_entities;
	std::vector<InteractableOpen*> m_interactableOpenCollection;
	std::vector<ItemUse*> m_itemUseCollection;
	std::vector<ItemPut*> m_itemPutCollection;

	void logWelcomeMessage();
	void logHelpMessage();

	template <typename T>
	void deleteCollection(std::vector<T>& collection);
};

extern World* world;

template<typename T>
void World::deleteCollection(std::vector<T>& collection)
{
	for (T element : collection)
	{
		delete element;
	}
	collection.clear();
}


#endif // !H_WORLD
