#ifndef H_INTERACTABLE_OPEN
#define H_INTERACTABLE_OPEN

#include<string>
class Interactable;
class ActionEffect;


struct InteractableOpen
{
public:
	Interactable* interactable;
	std::string m_description;
	ActionEffect* effect;
};


#endif // !H_INTERACTABLE_OPEN
