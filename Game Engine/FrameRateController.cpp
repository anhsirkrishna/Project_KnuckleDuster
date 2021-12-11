#include "FrameRateController.h"
#include "Util.h"
#include <string>

/*Specify the framerate in FPS*/
FrameRateController::FrameRateController(unsigned int frameRate) : mTimePerFrame(1000.0/frameRate), mLoopStartTime(0), 
																   mPrevLoopDeltaTime(0), frame_counter(0) {
}
FrameRateController::~FrameRateController() {

}

void FrameRateController::start_game_loop() {
	mLoopStartTime = SDL_GetTicks();
}

void FrameRateController::end_game_loop() {
	frame_counter++;
	limit_frame_rate();
	mPrevLoopDeltaTime = SDL_GetTicks() - mLoopStartTime;
	float avg_fps = frame_counter / (SDL_GetTicks() / 1000.0f);
	std::string log_msg = "FPS : " + std::to_string(avg_fps);
	DEBUG_LOG(log_msg);
}

void FrameRateController::limit_frame_rate() {
	Uint32 delayTime = std::max(0.0f, mTimePerFrame - (SDL_GetTicks() - mLoopStartTime));
	SDL_Delay(delayTime);
}

Uint32 FrameRateController::GetLoopStartTime() {
	return mLoopStartTime;
}

Uint32 FrameRateController::GetPrevLoopDeltaTime() {
	return mPrevLoopDeltaTime;
}