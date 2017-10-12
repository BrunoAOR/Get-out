#include "Application.h"
#include "globals.h"

int main()
{
	LoopStatus loopStatus = LoopStatus::Continue;

	// Start-up Application
	OutputLog("INFO: App will initiate.");
	Application app;
	loopStatus = app.init();

	OutputLog("INFO: App will update.");
	while (loopStatus == LoopStatus::Continue)
	{
		loopStatus = app.update();
	}

	// Close-up Application
	if (loopStatus == LoopStatus::Exit)
	{
		OutputLog("INFO: User requested App closure.");
		OutputLog("INFO: App will close.");
		loopStatus = app.close();
	}

	switch (loopStatus)
	{
	case LoopStatus::InitError:
		OutputLog("ERROR: App failed at initiation!");
		break;
	case LoopStatus::UpdateError:
		OutputLog("ERROR: App failed during update!");
		break;
	case LoopStatus::CloseError:
		OutputLog("ERROR: App failed at closure!");
		break;
	case LoopStatus::Continue:
		// We shouldn't hit this point if loopStatus is Continue
		OutputLog("ERROR: Main looped aborted!");
		break;
	case LoopStatus::Exit:
		OutputLog("INFO: App exited successfully!");
		break;
	default:
		break;
	}

	return 0;
}

// TODO: FIX BUG: Ctrl+C crashes app