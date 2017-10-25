#include "Application.h"

#include "World.h"
#include "InputReader.h"


Application::Application()
{
	m_inputReader = new InputReader();
}


Application::~Application()
{
	delete m_inputReader;
	m_inputReader = nullptr;
}

LoopStatus Application::init()
{
	world = new World();
	return world->init();
}


LoopStatus Application::update()
{
	// Adquire input and pass to World
	LoopStatus loopStatus = world->update(m_inputReader->getInput());
	if (loopStatus == LoopStatus::EXIT)
	{
		// TODO: Handle the game exit nicely (maybe a press to exit message or something of the sort)
	}
	return loopStatus;
}


LoopStatus Application::close()
{
	LoopStatus loopStatus = world->close();
	delete world;
	world = nullptr;
	return loopStatus;
}
