#ifndef H_INTERACTABLE
#define H_INTERACTABLE

#include "Entity.h"


class Interactable :
	public Entity
{
public:
	Interactable(EntityType type, std::string name, std::string description, std::string inspectDescription, bool isLocked, bool isVisibleInDark);
	~Interactable();

private:
	std::string m_inspectDescription;
	bool m_isLocked;
	bool m_isVisibleInDark;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_INTERACTABLE
