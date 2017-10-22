#ifndef H_ITEM
#define H_ITEM

#include "Entity.h"


class Item :
	public Entity
{
public:
	Item(std::string name, std::string m_description, std::string inspectDescription, bool isVisibleInDark = false, bool hasLight = false);
	~Item();

	virtual std::string getDescription() const override;
	virtual std::string getDetailedDescription() const override;

	bool hasLight() const;

private:
	std::string m_inspectDescription;
	bool m_hasLight;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_ITEM
