#ifndef H_ITEM
#define H_ITEM

#include "Entity.h"
#include "EntityFactory.h"


class Item :
	public Entity
{
	friend Entity * EntityFactory::createEntity(EntityInfo);
private:
	Item(int id, const std::string& name, const std::string& description, const std::string& inspectDescription, bool isVisibleInDark = false, bool hasLight = false);
	virtual ~Item();
public:
	Item(const Item& source) = delete;
	Item& operator=(const Item& source) = delete;

	virtual std::string getDescription() const override;
	virtual std::string getDetailedDescription() const override;

	bool hasLight() const;

private:
	std::string m_inspectDescription;
	bool m_hasLight;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_ITEM
