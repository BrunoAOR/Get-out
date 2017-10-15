#ifndef H_PLAYER
#define H_PLAYER

#include "Entity.h"
struct Instruction;
class Item;
class Interactable;
struct ItemUse;
struct ItemPut;
struct InteractableOpen;
enum class Direction;



class Player :
	public Entity
{
public:
	Player(EntityType type, std::string name, std::string description, unsigned int maxItems);
	~Player();

	void executeInstruction(const Instruction* instruction);

private:
	unsigned int m_maxItems = -1;

	virtual bool canAddChild(Entity* child) override;
	
	void look();
	bool go(Direction direction); // Ask room if there's an exit in that direkol,ction -> Room::getExit
	bool take(Entity* target); // Must check that type == Item && there’s room in children (inheritted from Entity)
	bool drop(Entity* target); // Must check that target is in children
	bool inspect(Entity* target);	// Item or interactable
	bool open(InteractableOpen* interactableOpen); //Must check type & presence in room
	bool use(ItemUse* itemUse); // Must verify that item can be used in target (done by item internally)
	bool put(ItemPut* itemPut); // Place an item inside another item (if allowed)
};


#endif // !H_PLAYER
