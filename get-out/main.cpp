#include "GameManager.h"
#include "globals.h"


int main()
{
	LoopStatus lLoopStatus = LoopStatus::CONTINUE;

	// Start-up Application
	OUTPUT_LOG("INFO: App will initiate.");
	GameManager lApp;
	lLoopStatus = lApp.init();

	if (lLoopStatus == LoopStatus::CONTINUE)
	{
		OUTPUT_LOG("INFO: App will update.");
	}
	while (lLoopStatus == LoopStatus::CONTINUE)
	{
		lLoopStatus = lApp.update();
	}

	// Close-up Application
	if (lLoopStatus == LoopStatus::EXIT)
	{
		OUTPUT_LOG("INFO: User requested App closure.");
		OUTPUT_LOG("INFO: App will close.");
		lLoopStatus = lApp.close();
	}

	switch (lLoopStatus)
	{
	case LoopStatus::INIT_ERROR:
		OUTPUT_LOG("ERROR: App failed at initiation!");
		break;
	case LoopStatus::UPDATE_ERROR:
		OUTPUT_LOG("ERROR: App failed during update!");
		break;
	case LoopStatus::CLOSE_ERROR:
		OUTPUT_LOG("ERROR: App failed while closing!");
		break;
	case LoopStatus::CONTINUE:
		// We shouldn't hit this point if lLoopStatus is Continue
		OUTPUT_LOG("ERROR: Main loop aborted!");
		break;
	case LoopStatus::EXIT:
		OUTPUT_LOG("INFO: App exited successfully!");
		break;
	default:
		break;
	}

	return !(lLoopStatus == LoopStatus::EXIT);
}
