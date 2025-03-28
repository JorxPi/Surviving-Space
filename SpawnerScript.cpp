#include "SpawnerScript.h"

void SpawnerScript::startScript()
{
}

void SpawnerScript::tickScript(float deltaTime)
{

	t += deltaTime;

	if (t > delay) {
		cout << "spawn" << endl;
		t = 0;

		Entity* ent = world->create();
		ent->assign<Transform3D>(glm::vec3(0., 0., 0.),0, 0,  1);
		ent->assign<MeshComponent>("Meshes/cube.obj", "Textures/science_dog.png");

		//Entity* ent2d = world->create();
		//ent2d->assign<Transform2D>(glm::vec2(100. + counter*100, 100.), 0.f, 1.f);
		//ent2d->assign<Sprite>("Textures/science_dog.png", glm::vec3(1., 1., 1.), false, glm::vec2(100., 100.), "unlit");

		//counter += 1;
	}
}
