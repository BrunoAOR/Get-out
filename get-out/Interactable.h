#ifndef H_INTERACTABLE
#define H_INTERACTABLE

#include "Entity.h"


class Interactable :
	public Entity
{
public:
	Interactable(std::string name, std::string m_description, std::string inspectDescription, bool isVisibleInDark);
	~Interactable();

	virtual std::string getDetailedDescription() const override;

private:
	std::string m_inspectDescription;
	bool m_isVisibleInDark;

	virtual bool canAddChild(Entity* child) override;
};


#endif // !H_INTERACTABLE
