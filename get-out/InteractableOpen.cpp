#include "InteractableOpen.h"
#include "ActionType.h"
#include "Interactable.h"
#include <assert.h>


InteractableOpen::InteractableOpen(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Interactable* interactable)
	: Action(ActionType::InteractableOpen, description, effects, shouldDestroy, interactable, nullptr)
{
	assert(interactable);
}


InteractableOpen::~InteractableOpen()
{
}
