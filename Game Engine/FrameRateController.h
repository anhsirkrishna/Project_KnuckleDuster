#pragma once
#include "SDL.h"
#include <algorithm>

class FrameRateController {
public:
	FrameRateController(unsigned int frameRate);
	~FrameRateController();
	/*Function called at the start of the game loop to record the time at the start*/
	void start_game_loop();
	/*Function called at the end of the game loop to get the loop end time and limit the framerate.*/
	void end_game_loop();
	/*Function called at the end of the game loop to limit the framerate*/
	void limit_frame_rate();
	Uint32 GetLoopStartTime();
private:
	float mTimePerFrame;
	Uint32 mLoopStartTime;
	Uint32 mPrevLoopDeltaTime;
};

extern FrameRateController* pFrameRateController;