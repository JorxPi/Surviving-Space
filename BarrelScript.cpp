#include "BarrelScript.h"

void BarrelScript::tickScript(float deltaTime) {


}

void BarrelScript::grabBarrel() {
	cout << "touching barrel" << endl;

	entity->get<Transform3D>()->position = glm::vec3(-500, -1.5, -500);
	barrelGrabbed = true;
}

void BarrelScript::resetBarrel() {
	entity->get<Transform3D>()->position = initialPos;
	barrelGrabbed = false;
}

void BarrelScript::setInitialPos(glm::vec3 initialPs) {
	initialPos = initialPs;
}
