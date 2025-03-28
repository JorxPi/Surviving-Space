#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ScriptManager.h"
#include "FirstPersonCameraScript.h"
#include "Script.h"
#include "TextScript.h"

#include "ECS.h"
#include "SpawnerScript.h"
#include "SkyboxScript.h"

using std::cout; 
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace ECS;

GLFWwindow* window; // Game window
const unsigned int width = 800;
const unsigned int height = 800;

float t = 0;
time_t current_time;

World* world;

void SetupGLFW() {

	glfwInit();

	// Tell GLFW we are using OpenGL 3.3 and the CORE profile (only the modern functions)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

bool SetupWindow() {
	//Create a GLFWwindow with size 800x800
	window = glfwCreateWindow(800, 800, "ProgramacioVideojocs", NULL, NULL);
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make window part of the current context
	glfwMakeContextCurrent(window);

	//Load GLAD and specify the viewport
	gladLoadGL();
	glViewport(0, 0, width, height);

	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

Entity* CreateEntity2D(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, 
	bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default") {
	Entity* ent = world->create();
	ent->assign<Transform2D>(position, rotation, scale);
	ent->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	return ent;
}


Entity* CreateEntity3DWithMesh(glm::vec3 position, float xrotation,float yrotation, float scale, const char* meshFilepath, const char* texFilepath, const char* normalsFilepath = "Textures/flat_normals.png") {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, xrotation, yrotation, scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath, "default", normalsFilepath);

	return ent;
}

Entity* CreateEntity3DEmpty() {
	Entity* ent = world->create();

	return ent;
}

Entity* CreateCamera(glm::vec3 position) {
	Entity* ent = world->create();
	ent->assign<Camera>(position, glm::vec3(0., 0., -1.), glm::vec3(0., 1., 0.));

	return ent;
}

Entity* CreateSkybox(const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Skybox>(texFilepath, meshFilepath);

	return ent;
}

void SetupWorld() {

	RenderSystem* rs = new RenderSystem(width, height);

	world = World::createWorld();
	world->registerSystem(rs);
	ScriptSystem* scriptSystem = new ScriptSystem();
	world->registerSystem(scriptSystem);

	ScriptManager* scriptManager = scriptSystem->getScriptManager();

	Entity* ent = CreateCamera(glm::vec3(30.0f, 2.f, 60.0f));
	FirstPersonCameraScript* fps = new FirstPersonCameraScript(window, world, ent);
	
	ent->assign<ScriptComponent>(scriptManager->AddScript(fps));

	rs->setCamera(ent);

	Entity* skybox = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/solar_system.png");

	

	string mapFloor[] = {
		"----------C________K--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------Z========X--------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"------------E((((R----------------------",
		"------------)----)----------------------",
		"--S------E((T----Y((R------O------------",
		"A--------)----------)-------------------",
		"---------Y((R----E((T----O--------------",
		"-----A------)----)----------------------",
		"------------Y((((T-----------O----------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"------------------O---------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
	};

	string mapRoof[] = {
		"----------C________K--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|---------C___K------",
		"----------!########|---------!###|------",
		"----------!########|---------Z===X------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------!########|--------------------",
		"----------Z========X--------------------",
		"----------------------------------------",
		"--------------CK------------------------",
		"--------------!|------------------------",
		"--------------!|------------------------",
		"----O---------ZX------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"------------------------O---------------",
		"------S---O--------O---------O----------",
		"----------------------------------------",
		"S--------------------O------------------",
		"---A------------------------------------",
		"--------------CK------------------------",
		"--------------ZX------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
		"----------------------------------------",
	};

	string mapNave[] = {
		"C____K",
		"!####|",
		"!####|",
		"!####|",
		"!####|",
		"Z====X",
	};

	Entity* small_plat1 = CreateEntity3DWithMesh(glm::vec3(19 * 4, -1.5, 25 * 4), 0, 0, 4, "Meshes/plataforma_elevada.obj", "Textures/Texturas_plataformas.png");
	small_plat1->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* small_plat2 = CreateEntity3DWithMesh(glm::vec3(2 * 4, -1.3, 16 * 4), 0, 0, 4, "Meshes/plataforma_elevada.obj", "Textures/Texturas_plataformas.png");
	small_plat2->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* big_plat1 = CreateEntity3DWithMesh(glm::vec3(2 * 4, -1.5, 17 * 4), 0, 0, 4, "Meshes/plataforma_elevada_extra.obj", "Textures/Texturas_plataformas.png");
	big_plat1->assign<CubeCollider>(2.1, 4.1, 2.1);

	Entity* big_plat2 = CreateEntity3DWithMesh(glm::vec3(5 * 4, -1.5, 18 * 4), 0, 0, 4, "Meshes/plataforma_elevada_extra.obj", "Textures/Texturas_plataformas.png");
	big_plat2->assign<CubeCollider>(2.1, 4.1, 2.1);

	Entity* plat1 = CreateEntity3DWithMesh(glm::vec3(6 * 4, 3, 21 * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
	plat1->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat2 = CreateEntity3DWithMesh(glm::vec3(5 * 4, 4, 24 * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
	plat2->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat3 = CreateEntity3DWithMesh(glm::vec3(5 * 4, 6, 27 * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
	plat3->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* big_coll1 = CreateEntity3DWithMesh(glm::vec3(8 * 4, -2, 12 * 4), 0, 0, 4, "Meshes/columna_big.obj", "Textures/Texturas_plataformas.png");
	big_coll1->assign<CubeCollider>(1.6, 5.1, 1.6);

	Entity* small_coll1 = CreateEntity3DWithMesh(glm::vec3(6 * 4, 30, 13 * 4), 180, 0, 4, "Meshes/columna_small.obj", "Textures/Texturas_plataformas.png");
	small_coll1->assign<CubeCollider>(2.1, 3.1, 2.1);

	Entity* small_coll2 = CreateEntity3DWithMesh(glm::vec3(5 * 4, -2, 10 * 4), 0, 0, 4, "Meshes/columna_small.obj", "Textures/Texturas_plataformas.png");
	small_coll2->assign<CubeCollider>(1.6, 3.1, 1.6);

	Entity* barrel1 = CreateEntity3DWithMesh(glm::vec3(21 * 4, -1, 29 * 4), 0, 0, 6, "Meshes/barril.obj", "Textures/Texturas_plataformas.png");
	barrel1->assign<CubeCollider>(1.1, 2.1, 1.1, 1); 
	barrel1->get<MeshComponent>()->shaderName = "barrel";
	BarrelScript* barrel1_script = new BarrelScript(window, world, barrel1);
	barrel1_script->setInitialPos(glm::vec3(21 * 4, -1, 29 * 4));
	barrel1->assign<ScriptComponent>(scriptManager->AddScript(barrel1_script));

	Entity* barrel2 = CreateEntity3DWithMesh(glm::vec3(14 * 4, 28, 4 * 4), 0, 0, 6, "Meshes/barril.obj", "Textures/Texturas_plataformas.png");
	barrel2->assign<CubeCollider>(1.1, 2.1, 1.1, 1);
	barrel2->get<MeshComponent>()->shaderName = "barrel";
	BarrelScript* barrel2_script = new BarrelScript(window, world, barrel2);
	barrel2_script->setInitialPos(glm::vec3(14 * 4, 28, 4 * 4));
	barrel2->assign<ScriptComponent>(scriptManager->AddScript(barrel2_script));

	Entity* plat4 = CreateEntity3DWithMesh(glm::vec3(25 * 4, 15, 14 * 4), 0, 270, 4, "Meshes/plataforma_end.obj", "Textures/Texturas_plataformas.png");
	plat4->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat5 = CreateEntity3DWithMesh(glm::vec3(25 * 4, 15, 15 * 4), 0, 90, 4, "Meshes/plataforma_end.obj", "Textures/Texturas_plataformas.png");
	plat5->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat6 = CreateEntity3DWithMesh(glm::vec3(28 * 4, 11, 14 * 4), 0, 0, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
	plat6->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat7 = CreateEntity3DWithMesh(glm::vec3(28 * 4, 11, 15 * 4), 0, 90, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
	plat7->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat8 = CreateEntity3DWithMesh(glm::vec3(29 * 4, 11, 14 * 4), 0, 270, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
	plat8->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* plat9 = CreateEntity3DWithMesh(glm::vec3(29 * 4, 11, 15 * 4), 0, 180, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
	plat9->assign<CubeCollider>(2.1, 1.1, 2.1);

	Entity* barrels1 = CreateEntity3DWithMesh(glm::vec3(28.5 * 4, 10, 14.5 * 4), 180, 0, 6, "Meshes/barriles.obj", "Textures/Texturas_plataformas.png");
	barrels1->assign<CubeCollider>(2.1, 1.1, 2.1, 1);
	barrels1->get<MeshComponent>()->shaderName = "barrel";
	BarrelScript* barrels1_script = new BarrelScript(window, world, barrels1);
	barrels1_script->setInitialPos(glm::vec3(28.5 * 4, 10, 14.5 * 4));
	barrels1->assign<ScriptComponent>(scriptManager->AddScript(barrels1_script));

	Entity* gate1 = CreateEntity3DWithMesh(glm::vec3(4 * 4, 30, 31 * 4), 180, 0, 12, "Meshes/gate.obj", "Textures/Texturas_plataformas.png");
	gate1->assign<CubeCollider>(6.1, 8.1, 3.1);

	Entity* nave1 = CreateEntity3DWithMesh(glm::vec3((2.5 * 4) + 5, -4.7, (4 * 4) + 140), 0, 180, 6, "Meshes/nave.obj", "Textures/text_nave.png");
	nave1->assign<CubeCollider>(10.1, 7.1, 10.1, 2);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			if (mapNave[i][j] == '#') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 0, 4, "Meshes/plataforma_normal.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}

			if (mapNave[i][j] == 'C') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 0, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}

			if (mapNave[i][j] == 'K') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 90, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapNave[i][j] == 'X') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 180, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapNave[i][j] == 'Z') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 270, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapNave[i][j] == '_') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 90, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapNave[i][j] == '|') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 180, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapNave[i][j] == '=') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 270, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapNave[i][j] == '!') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3((i * 4) + 5, -5, (j * 4) + 140), 0, 0, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
		}
	}

	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 40; j++) {
			if (mapFloor[i][j] == '#') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_normal.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}

			if (mapFloor[i][j] == 'C') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}

			if (mapFloor[i][j] == 'K') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 90, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'X') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 180, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'Z') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 270, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == '_') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 90, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == '|') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 180, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == '=') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 270, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == '!') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'E') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_corner_extra.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'R') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 90, 4, "Meshes/plataforma_corner_extra.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'T') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 180, 4, "Meshes/plataforma_corner_extra.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'Y') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 270, 4, "Meshes/plataforma_corner_extra.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == '(') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 90, 4, "Meshes/plataforma_straight.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == ')') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_straight.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'O') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'S') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'S') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/columna_small.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(1.6, 3.1, 1.6);
			}
			if (mapFloor[i][j] == 'A') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapFloor[i][j] == 'A') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, -2, j * 4), 0, 0, 4, "Meshes/columna_big.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(1.6, 5.1, 1.6);
			}
		}
	}


	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 40; j++) {
			if (mapRoof[i][j] == '#') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 0, 4, "Meshes/plataforma_normal.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}

			if (mapRoof[i][j] == 'C') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 0, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}

			if (mapRoof[i][j] == 'K') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 90, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'X') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 180, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'Z') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 270, 4, "Meshes/plataforma_corner.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == '_') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 90, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == '|') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 180, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == '=') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 270, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == '!') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 0, 4, "Meshes/plataforma_side.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'O') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'G') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 270, 4, "Meshes/plataforma_end.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == '(') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 90, 4, "Meshes/plataforma_straight.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'H') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 90, 4, "Meshes/plataforma_end.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'S') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'S') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 180, 0, 4, "Meshes/columna_small.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 3.1, 2.1);
			}
			if (mapRoof[i][j] == 'A') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 0, 0, 4, "Meshes/plataforma_unic.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 1.1, 2.1);
			}
			if (mapRoof[i][j] == 'A') {
				Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 4, 30, j * 4), 180, 0, 4, "Meshes/columna_big.obj", "Textures/Texturas_plataformas.png");
				wall->assign<CubeCollider>(2.1, 5.1, 2.1);
			}
		}
	}

	Entity* casco_outlier = CreateEntity2D(glm::vec2(400.f, 400.f), 0.f, 1.f, "Textures/casco_astronauta.png", glm::vec3(1., 1., 1.), false, glm::vec2(1130., 910.));

	Entity* text1 = CreateEntity2D(glm::vec2(400.f, 350.f), 0.f, 1.f, "Textures/congrats.png", glm::vec3(1., 1., 1.), false, glm::vec2(494., 102.));
	text1->get<Sprite>()->visible = false;

	TextScript* text1_script = new TextScript(window, world, text1);
	text1->assign<ScriptComponent>(scriptManager->AddScript(text1_script));

	Entity* text2 = CreateEntity2D(glm::vec2(400.f, 450.f), 0.f, 1.f, "Textures/surviving_space.png", glm::vec3(1., 1., 1.), false, glm::vec2(668., 94.));
	text2->get<Sprite>()->visible = false;

	TextScript* text2_script = new TextScript(window, world, text2);
	text2->assign<ScriptComponent>(scriptManager->AddScript(text2_script));

	//Entity* casco_outlier = CreateEntity2D(glm::vec2(400.f, 400.f), 0.f, 1.f, "Textures/casco_astronauta.png", glm::vec3(1., 1., 1.), false, glm::vec2(100., 100.));

	Entity* gravity_icon = CreateEntity2D(glm::vec2(750.f, 50.f), 0.f, 1.f, "Textures/gravity_icon.png", glm::vec3(1., 1., 1.), false, glm::vec2(92., 92.));

	GravityScript* gravity_icon_script = new GravityScript(window, world, gravity_icon);

	fps->setGravityScriptRef(gravity_icon_script);
	fps->setScriptRef(scriptManager);

	fps->setTextsRef(text1_script, text2_script);
}

int main() {
	
	SetupGLFW();

	if (!SetupWindow()) {
		return -1;
	}

	SetupWorld();

	float dt = 0;
	float time = clock();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Program core loop
	while (!glfwWindowShouldClose(window)) {

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dt = clock() - time;
		time = clock();
		if (dt < 50) {
			world->tick(dt);
		}

		glfwSwapBuffers(window); //Swap buffers

		// Take care of GLFW events
		glfwPollEvents();
	}

	// Cleanup

	glfwDestroyWindow(window);
	glfwTerminate();

	world->destroyWorld();

	return 0;
}