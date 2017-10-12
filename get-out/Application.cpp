#include "Application.h"

#include "World.h"
#include "InputReader.h"


Application::Application()
{
	world = new World();
	inputReader = new InputReader();
}


Application::~Application()
{
	delete world;
	world = nullptr;
	delete inputReader;
	inputReader = nullptr;
}

LoopStatus Application::init()
{
	return world->init();
}


LoopStatus Application::update()
{
	// Adquire input and pass to World
	return world->update(inputReader->getInput());
}


LoopStatus Application::close()
{
	return world->close();
}
