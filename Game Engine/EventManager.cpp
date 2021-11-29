#include "EventManager.h"
#include "GameObjectManager.h"

//Initializes the subscriber list with the different types of events
EventManager::EventManager() {
	for (const auto e : all_events) {
		subscriber_list[e] = std::vector<GameObject*>();
	}
}


//Broadcasts an event to all objects in the game
void EventManager::BroadcastEvent(TimedEvent* p_event) {
	for (unsigned int i = 0; i < pGameObjectManager->max_objects; i++) {
		if (pGameObjectManager->game_object_list[i] == NULL)
			continue;
		pGameObjectManager->game_object_list[i]->HandleEvent(p_event);
	}
}

//Broadcasts an event to only objects that are subscribed to receive events of a particular type.
void EventManager::BroadcastEventToSubscribers(TimedEvent* p_event) {
	auto subscribed_objects = subscriber_list[p_event->event_id];
	for (auto object : subscribed_objects) {
		object->HandleEvent(p_event);
	}
}

//Add a timed event to the list of events that need to be triggered.
void EventManager::QueueTimedEvent(TimedEvent* p_event) {
	timed_event_list.push_back(p_event);
}

//Allows a game object to subscribe to receive particular types of events
void EventManager::SubscribeToEvent(EventID event_type, GameObject* p_game_object) {
	subscriber_list[event_type].push_back(p_game_object);
}


//Called once every game loop
//Checks if it's time for any of the events to be triggered 
//If it's time then it broadcasts it appropriately.
void EventManager::Update() {
	std::vector<TimedEvent*>::iterator it = timed_event_list.begin();
	while (it != timed_event_list.end()) {
		TimedEvent* timed_event = *it;
		if (timed_event->When() <= 0) {
			if (timed_event->broadcast)
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