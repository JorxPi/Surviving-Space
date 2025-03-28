#include "TextScript.h"

void TextScript::turnVisible(bool vis) {
	ComponentHandle<Sprite> sprite = entity->get<Sprite>();

	sprite->visible = vis;
}

