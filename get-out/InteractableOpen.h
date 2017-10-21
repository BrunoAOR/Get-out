#ifndef H_INTERACTABLE_OPEN
#define H_INTERACTABLE_OPEN

#include<string>
#include<vector>
class Interactable;
class ActionEffect;


class InteractableOpen
{
public:
	InteractableOpen(std::string description, std::vector<ActionEffect*> effects, bool shouldDestroy, Interactable* interactable);
	~InteractableOpen();

	const Interactable* getInteractable() const;
	std::string getDescription() const;
	void performEffects();

private:
	std::string m_description;
	std::vector<ActionEffect*> m_effects;
	bool m_shouldDestroy;
	Interactable* m_interactable;
};


#endif // !H_INTERACTABLE_OPEN
