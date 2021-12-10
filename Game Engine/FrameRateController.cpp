#include "FrameRateController.h"

/*Specify the framerate in FPS*/
FrameRateController::FrameRateController(unsigned int frameRate) {
	mTimePerFrame = (1.0/frameRate)*1000;
	mLoopStartTime = 0;
	mPrevLoopDeltaTime = 0;
}
FrameRateController::~FrameRateController() {

}

void FrameRateController::start_game_loop() {
	mLoopStartTime = SDL_GetTicks();
}

void FrameRateController::end_game_loop() {
	limit_frame_rate();
	mPrevLoopDeltaTime = SDL_GetTicks() - mLoopStartTime;
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