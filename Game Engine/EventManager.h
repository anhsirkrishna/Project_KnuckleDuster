#pragma once
#include <vector>
#include <unordered_map>
#include "Events.h"

class GameObject;

class EventManager {
public:
	EventManager();
	void BroadcastEvent(TimedEvent* p_event);
	void BroadcastEventToSubscribers(TimedEvent* p_event);
	void QueueTimedEvent(TimedEvent* p_event);
	void Update();

	void SubscribeToEvent(EventID event_type, GameObject* p_game_object);
private:
	std::vector<TimedEvent*> timed_event_list;
	std::unordered_map<EventID, std::vector<GameObject*>> subscriber_list;
};

extern EventManager* p_event_manager;