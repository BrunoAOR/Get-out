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
	return world->update(m_inputReader->getInput());
}


LoopStatus Application::close()
{
	return world->close();
}
