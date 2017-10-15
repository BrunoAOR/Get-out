#ifndef H_APPLICATION
#define H_APPLICATION

#include "globals.h"

class InputReader;


class Application
{
public:
	Application();
	~Application();

	LoopStatus init();
	LoopStatus update();
	LoopStatus close();

private:
	InputReader* m_inputReader = nullptr;
};


#endif // !H_APPLICATION
