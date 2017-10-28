#include "Application.h"

#include "InputReader.h"
#include "World.h"


Application::Application()
{
	m_inputReader = new InputReader();
	m_world = new World();
}


Application::~Application()
{
	delete m_world;
	m_world = nullptr;
	delete m_inputReader;
	m_inputReader = nullptr;
}


LoopStatus Application::init()
{
	return m_world->init();
}


LoopStatus Application::update()
{
	// Adquire input and pass to World
	if (m_loopStatus != LoopStatus::EXIT)
	{
		m_loopStatus = m_world->update(m_inputReader->getInput());
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
	return m_world->close();
}
