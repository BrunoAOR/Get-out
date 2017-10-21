#ifndef H_PLAYER
#define H_PLAYER

#include "Entity.h"
struct Instruction;
class Room;


class Player :
	public Entity
{
public:
	Player(std::string name, std::string m_description, unsigned int maxItems, Room* startingRoom);
	~Player();

	void executeInstruction(const Instruction* instruction);

private:
	Room* m_location;
	unsigned int m_maxItems = -1;

	virtual bool canAddChild(Entity* child) override;
	
	void look();
	void inventory();
	bool go(const Instruction* instruction); // Ask room if there's an exit in that direkol,ction -> Room::getExit
	bool take(const Instruction* instruction); // Must check that type == Item && there’s room in children (inheritted from Entity)
	bool drop(const Instruction* instruction); // Must check that target is in children
	bool inspect(const Instruction* instruction);	// Item or interactable
	bool open(const Instruction* instruction); //Must check type & presence in room
	bool use(const Instruction* instruction); // Must verify that item can be used in target (done by item internally)
	bool put(const Instruction* instruction); // Place an item inside another item (if allowed)
};


#endif // !H_PLAYER
