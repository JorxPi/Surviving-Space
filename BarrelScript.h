#pragma once

#include "Script.h"

class BarrelScript : public Script
{
	using Script::Script;

public:
	void tickScript(float deltaTime) override;
	void grabBarrel();
	void resetBarrel();
	void setInitialPos(glm::vec3 initialPos);
private:
	glm::vec3 initialPos;
	bool touched = false;
	bool barrelGrabbed = false;
};