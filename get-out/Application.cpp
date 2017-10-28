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
	if (m_loopStatus != LoopStatus::EXIT)
	{
		m_loopStatus = world->update(m_inputReader->getInput());
		if (m_loopStatus == LoopStatus::EXIT)
		{
			consoleLog("Thanks for playing GET OUT!\n(Press Enter to close the window)");
		}
	}
	else
	{
		if (m_inputReader->getEnter())
		{
			return LoopStatus::EXIT;
		}
	}
	return LoopStatus::CONTINUE;
}


LoopStatus Application::close()
{
	LoopStatus loopStatus = world->close();
	delete world;
	world = nullptr;
	return loopStatus;
}
