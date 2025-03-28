#include "GravityScript.h"

void GravityScript::tickScript(float deltaTime) {
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();

	
}

void GravityScript::changeSprite(bool gravityDirection) {
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();
	if (gravityDirection) {
		sprite->filepath = "Textures/gravity_icon.png";
	}
	else {
		sprite->filepath = "Textures/gravity_icon_2.png";
	}
}