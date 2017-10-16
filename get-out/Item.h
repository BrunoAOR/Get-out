#ifndef H_ITEM
#define H_ITEM

#include "Entity.h"


class Item :
	public Entity
{
public:
	Item(EntityType type, std::string name, std::string description, std::string inspectDescription, bool hasLight);
	~Item();

	virtual std::string getDetailedDescription() const override;

private:
	std::string m_inspectDescription;
	bool m_hasLight;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_ITEM
