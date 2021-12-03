#pragma once
#include <vector>
#include <unordered_map>
#include "Events.h"

class GameObject;

class EventManager {
public:
	EventManager();
	void QueueTimedEvent(TimedEvent* p_event);
	void Update();
	void SubscribeToEvent(EventID event_type, GameObject* p_game_object);
private:
	std::vector<TimedEvent*> timed_event_list;
	std::vector<TimedEvent*> new_events;
	std::unordered_map<EventID, std::vector<int>> subscriber_list;
	void BroadcastEvent(TimedEvent* p_event);
	void BroadcastEventToSubscribers(TimedEvent* p_event);
	void SendEvent(TimedEvent* p_event, int object_index);
};

extern EventManager* p_event_manager;