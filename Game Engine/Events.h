#pragma once

//Enum of the different types of events
enum class EventID
{
	collision,
	out_of_bounds,
	hit,
	health_zero,
	take_damage,
	knock_down,
	downed,
	revived,
	NUM
};

static const EventID all_events[] = { EventID::collision,
	EventID::out_of_bounds,
	EventID::hit,
	EventID::health_zero,
	EventID::take_damage,
	EventID::knock_down,
	EventID::downed,
	EventID::revived,
	EventID::NUM };

class TimedEvent {
public:
	const EventID event_id;
	const bool broadcast;
	const int reciever_obj_index;
	TimedEvent(EventID _id, bool _broadcast=false, int _obj_index=-1);
	//virtual void Execute() = 0;
	virtual void SetTime(double _time);
	virtual double When() const;
	bool operator < (const TimedEvent& rhs);
	virtual ~TimedEvent();
private:
	double time;
};

class HitEvent : public TimedEvent {
public :
	HitEvent(unsigned int _dmg, int _direction, int _distance, int _obj_index, bool _broadcast=false);
	unsigned int hit_damage;
	int direction;
	int hit_move_distance;
};

class TakeDamageEvent : public TimedEvent {
public:
	TakeDamageEvent(unsigned int _dmg, int _obj_index, bool _broadcast = false);
	unsigned int hit_damage;
};

class DownedEvent : public TimedEvent {
public:
	DownedEvent(int _obj_index, bool _broadcast = false);
	unsigned int downed_obj_index;
};