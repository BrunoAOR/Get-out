#include "World.h"



World::World()
{
}


World::~World()
{
}


LoopStatus World::init()
{
	return LoopStatus::Continue;
}

#include <iostream>
LoopStatus World::update(const std::string& userInput)
{
	if (userInput != "")
	{
		// TODO: Replace testing lines with actual usage of InputParser
		consoleLog("Input received: " + userInput + '\n');
		if (caseLessEquals(userInput, "QUIT"))
		{
			return LoopStatus::Exit;
		}
	}
	return LoopStatus::Continue;
}


LoopStatus World::close()
{
	return LoopStatus::Exit;
}
