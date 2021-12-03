#include "EventManager.h"
#include "GameObjectManager.h"

//Initializes the subscriber list with the different types of events
EventManager::EventManager() {
	for (const auto e : all_events) {
		subscriber_list[e] = std::vector<int>();
	}
}

//Sends event to a specific object
void EventManager::SendEvent(TimedEvent* p_event, int object_index) {
	auto obj_to_send = pGameObjectManager->game_object_list[object_index];
	if (obj_to_send != NULL)
		obj_to_send->HandleEvent(p_event);
}

//Broadcasts an event to all objects in the game
void EventManager::BroadcastEvent(TimedEvent* p_event) {
	for (unsigned int i = 0; i < pGameObjectManager->max_objects; i++) {
		if (pGameObjectManager->game_object_list[i] == NULL)
			continue;
		SendEvent(p_event, i);
	}
}

//Broadcasts an event to only objects that are subscribed to receive events of a particular type.
void EventManager::BroadcastEventToSubscribers(TimedEvent* p_event) {
	auto subscribed_objects = subscriber_list[p_event->event_id];
	for (auto object_index : subscribed_objects) {
		SendEvent(p_event, object_index);
	}
}

//Add a timed event to the list of events that need to be triggered.
void EventManager::QueueTimedEvent(TimedEvent* p_event) {
	new_events.push_back(p_event);
}

//Allows a game object to subscribe to receive particular types of events
void EventManager::SubscribeToEvent(EventID event_type, GameObject* p_game_object) {
	subscriber_list[event_type].push_back(p_game_object->index);
}


//Called once every game loop
//Checks if it's time for any of the events to be triggered 
//If it's time then it broadcasts it appropriately.
void EventManager::Update() {
	for (auto new_event : new_events)
		timed_event_list.push_back(new_event);
	new_events.clear();
	std::vector<TimedEvent*>::iterator it = timed_event_list.begin();
	while (it != timed_event_list.end()) {
		TimedEvent* timed_event = *it;
		if (timed_event->When() <= 0) {
			if (timed_event->reciever_obj_index != -1)
				SendEvent(timed_event, timed_event->reciever_obj_index);
			else if (timed_event->broadcast)
				BroadcastEvent(timed_event);
			else
				BroadcastEventToSubscribers(timed_event);

			delete timed_event;
			it = timed_event_list.erase(it);
		}
		else
			++it;
	}
}