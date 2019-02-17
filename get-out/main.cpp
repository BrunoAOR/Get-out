#include "GameManager.h"
#include "globals.h"


int main()
{
	LoopStatus lLoopStatus = LoopStatus::CONTINUE;

	// Start-up Application
	OutputLog("INFO: App will initiate.");
	GameManager lApp;
	lLoopStatus = lApp.init();

	if (lLoopStatus == LoopStatus::CONTINUE)
	{
		OutputLog("INFO: App will update.");
	}
	while (lLoopStatus == LoopStatus::CONTINUE)
	{
		lLoopStatus = lApp.update();
	}

	// Close-up Application
	if (lLoopStatus == LoopStatus::EXIT)
	{
		OutputLog("INFO: User requested App closure.");
		OutputLog("INFO: App will close.");
		lLoopStatus = lApp.close();
	}

	switch (lLoopStatus)
	{
	case LoopStatus::INIT_ERROR:
		OutputLog("ERROR: App failed at initiation!");
		break;
	case LoopStatus::UPDATE_ERROR:
		OutputLog("ERROR: App failed during update!");
		break;
	case LoopStatus::CLOSE_ERROR:
		OutputLog("ERROR: App failed while closing!");
		break;
	case LoopStatus::CONTINUE:
		// We shouldn't hit this point if lLoopStatus is Continue
		OutputLog("ERROR: Main loop aborted!");
		break;
	case LoopStatus::EXIT:
		OutputLog("INFO: App exited successfully!");
		break;
	default:
		break;
	}

	return !(lLoopStatus == LoopStatus::EXIT);
}
