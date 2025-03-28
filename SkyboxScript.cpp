#include "SkyboxScript.h"



SkyboxScript::SkyboxScript(GLFWwindow* window, World* world, Entity* entity, Entity* camera)
{
	this->window = window;
	this->entity = entity;
	this->world = world;
	this->camera = camera;
}

void SkyboxScript::startScript() {

}

void SkyboxScript::tickScript(float deltaTime) {

	//ComponentHandle<Transform3D> myTransform = entity->get<Transform3D>();
	//ComponentHandle<Camera> cameraComp = camera->get<Camera>();

	//myTransform->position = cameraComp->position;

}