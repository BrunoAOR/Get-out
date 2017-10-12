#ifndef H_APPLICATION
#define H_APPLICATION

#include "globals.h"

class World;
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
	World* world = nullptr;
	InputReader* inputReader = nullptr;
};


#endif // !H_APPLICATION
