#include "Events.h"
#include "FrameRateController.h"

TimedEvent::TimedEvent(EventID _id, bool _broadcast) : time(0), event_id(_id), broadcast(_broadcast) { }

TimedEvent::~TimedEvent() {

}

//Set the time the event should be trigger. 
//Time is specified as number of milliseconds from the time of calling this function
void TimedEvent::SetTime(double _time) {
	time = pFrameRateController->GetLoopStartTime() + _time;
}

//Returns the number of milliseconds until the event needs to be triggered
//Event needs to be triggered when the returned value is 0 or less
double TimedEvent::When() const {
	return  time - pFrameRateController->GetLoopStartTime() ;
}

//If two events are compared to see which one needs to be executed quicker
bool TimedEvent::operator<(const TimedEvent& rhs) {
	return time < rhs.time;
}
