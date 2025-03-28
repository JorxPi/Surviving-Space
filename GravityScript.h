#pragma once

#include "Script.h"

class GravityScript : public Script
{
	using Script::Script;

public:
	void tickScript(float deltaTime) override;
	void changeSprite(bool gravityDirection);
private:
};