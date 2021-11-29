#include "Sprite.h"
#include "GameObject.h"

SDL_Surface* Sprite::GetImage() {
	return p_sprite_image;
}

void Sprite::SetImage(SDL_Surface* p_sprite) {
	p_sprite_image = p_sprite;
}

void Sprite::Draw(SDL_Surface* surface) {
	GameObject* p_owner_object = GetOwner();
	Transform* obj_transform = static_cast<Transform*>(p_owner_object->HasComponent("TRANSFORM"));
	SDL_BlitSurface(GetImage(), NULL, surface, &obj_transform->GetPosition());
}

void Sprite::Serialize(json json_object) {
	auto filename = json_object["filename"].get<std::string>();
	p_sprite_image = pResourceManager->get_resource(filename);
}