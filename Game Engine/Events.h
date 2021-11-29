#pragma once

//Enum of the different types of events
enum class EventID
{
	collision,
	out_of_bounds,

	NUM
};

static const EventID all_events[] = { EventID::collision, EventID::out_of_bounds, EventID::NUM };

class TimedEvent {
public:
	const EventID event_id;
	const bool broadcast;
	TimedEvent(EventID _id, bool _broadcast=false);
	//virtual void Execute() = 0;
	virtual void SetTime(double _time);
	virtual double When() const;
	bool operator < (const TimedEvent& rhs);
	virtual ~TimedEvent();
private:
	double time;
};