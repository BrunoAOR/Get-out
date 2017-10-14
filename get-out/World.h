#ifndef H_WORLD
#define H_WORLD

#include <vector>
#include <string>

enum class LoopStatus;
class InputParser;
struct NamesInfo;
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

	Entity* getEntity(const std::string& name);
	InteractableOpen* getInteractableOpen(const std::string& interactableName);
	ItemUse* getItemUse(const std::string& itemName, const std::string& interactableName);
	ItemPut* getItemPut(const std::string& itemName, const std::string& containerItemName);

private:
	InputParser* inputParser = nullptr;
	NamesInfo* namesInfo = nullptr;
	std::vector<Entity*> entities;
	std::vector<InteractableOpen*> interactableOpenCollection;
	std::vector<ItemUse*> itemUseCollection;
	std::vector<ItemPut*> itemPutCollection;

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
