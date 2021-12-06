#include "Hostage.h"
#include "GameObject.h"
#include "GLSprite.h"
#include "Events.h"
#include "GameManager.h"

Hostage::Hostage() : Component("HOSTAGE"), p_owner_glsprite(NULL) {

}

void Hostage::Serialize(json json_object) {

}

void Hostage::Link() {
	p_owner_glsprite = static_cast<GLSprite*>(GetOwner()->HasComponent("GLSPRITE"));
}

void Hostage::HandleEvent(TimedEvent* p_event) {
	switch (p_event->event_id) {
	case EventID::downed:
		p_game_manager->SetLevel(-1);
		break;
	}
}