#include "ComponentFactory.h"
#include "ControllerCreator.h"
#include "TransformCreator.h"
#include "SpriteCreator.h"
#include "UpDownCreator.h"
#include "GLSpriteCreator.h"
#include "HurtboxCreator.h"

ComponentFactory::ComponentFactory() {
	creator_map.insert(std::pair<std::string, ComponentCreator*>("TRANSFORM", new TransformCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("CONTROLLER", new ControllerCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("SPRITE", new SpriteCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("UPDOWN", new UpDownCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("GLSPRITE", new GLSpriteCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("HURTBOX", new HurtboxCreator));
}

ComponentFactory::~ComponentFactory() {
	for (std::pair<std::string, ComponentCreator*> element : creator_map)
	{
		delete element.second;
	}
}

Component* ComponentFactory::Create(std::string component_name, json json_object) {
	return creator_map.at(component_name)->Create(json_object);
}