#ifndef H_WORLD
#define H_WORLD

#include <string>
#include "globals.h"


class World
{
public:
	World();
	~World();

	LoopStatus init();
	LoopStatus update(const std::string& userInput);
	LoopStatus close();

private:
	// TODO: Add InputParser
};


#endif // !H_WORLD
