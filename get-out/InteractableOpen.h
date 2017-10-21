#ifndef H_INTERACTABLE_OPEN
#define H_INTERACTABLE_OPEN

#include "Action.h"
#include <string>
#include <vector>
class Interactable;
class ActionEffect;


class InteractableOpen :
	public Action
{
public:
	InteractableOpen(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Interactable* interactable);
	~InteractableOpen();
};


#endif // !H_INTERACTABLE_OPEN
