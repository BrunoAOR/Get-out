#include "Application.h"

#include "World.h"
#include "InputReader.h"


Application::Application()
{
	world = new World();
	m_inputReader = new InputReader();
}


Application::~Application()
{
	delete world;
	world = nullptr;
	delete m_inputReader;
	m_inputReader = nullptr;
}

LoopStatus Application::init()
{
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
	return world->close();
}
