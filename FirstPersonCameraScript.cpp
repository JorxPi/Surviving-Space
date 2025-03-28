#include "FirstPersonCameraScript.h"
#include <chrono>

void FirstPersonCameraScript::startScript() {

}

void FirstPersonCameraScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	float width = 800;
	float height = 800;
	
	ComponentHandle<Camera> cam = entity->get<Camera>();

	glm::vec3 currentPosition = cam->position;
	glm::vec3 desiredPosition = cam->position;

	if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) && !disableMovement)
	{
		desiredPosition.x += speedDelta * cam->orientation.x;
		desiredPosition.z += speedDelta * cam->orientation.z;
	}
	if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && !disableMovement)
	{
		desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up));
	}
	if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && !disableMovement)
	{
		desiredPosition.x -= speedDelta * cam->orientation.x;
		desiredPosition.z -= speedDelta * cam->orientation.z;
	}
	if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) && !disableMovement)
	{
		desiredPosition += speedDelta * glm::normalize(glm::cross(cam->orientation, cam->up));
	}


	if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) && !disableMovement)
	{
		if (!isFalling) {
			if (gravityDirection) {
				gravityDirection = false;
				gravity = -0.002f;
				isFalling = true;
				gravityRef->changeSprite(gravityDirection);
				//cam->up = glm::vec3(0, -1, 0);
			}
			else {
				gravityDirection = true;
				gravity = 0.002f;
				isFalling = true;
				gravityRef->changeSprite(gravityDirection);
				//cam->up = glm::vec3(0, 1, 0);
			}
		}
	}

	const float jumpStrength = 0.10f;
	const float minFloor = -50.0f;
	const float maxRoof = 100.0f;

	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !disableMovement)
	{
		if (!isFalling) {
			if (gravityDirection) {
				verticalVelocity = jumpStrength;
				isFalling = true;
			}
			else {
				verticalVelocity = -jumpStrength;
				isFalling = true;
			}
		}
	}

	desiredPosition.y += verticalVelocity * deltaTime/10;

	verticalVelocity -= gravity * deltaTime/10;
	
	if (desiredPosition.y < minFloor || desiredPosition.y > maxRoof) {
		desiredPosition = glm::vec3(30.0f, 7.f, 60.0f);
		cam->position = desiredPosition;
		isFalling = false;
		verticalVelocity = 0;
		gravityDirection = true;
		gravity = 0.002f;
		gravityRef->changeSprite(gravityDirection);
		totalBarrels = 0;

		world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {
			if (cubeColl->colliderId == 1) {
				ComponentHandle<ScriptComponent> script = ent->get<ScriptComponent>();
				BarrelScript* barrelScript = (BarrelScript*)sm->getScript(script->scriptId);
				barrelScript->resetBarrel();
			}
		});
	}

	glm::vec3 playerFeet = desiredPosition - glm::vec3(0, 5, 0);
	glm::vec3 playerHead = desiredPosition + glm::vec3(0, 5, 0);

	world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {

		glm::vec3 pos = ent->get<Transform3D>()->position;

		if (gravityDirection) {
			if (playerFeet.x < pos.x + cubeColl->width && playerFeet.x > pos.x - cubeColl->width &&
				playerFeet.y < pos.y + cubeColl->height && playerFeet.y > pos.y - cubeColl->height &&
				playerFeet.z < pos.z + cubeColl->length && playerFeet.z > pos.z - cubeColl->length) {

				if (cubeColl->colliderId == 1) {
					ComponentHandle<ScriptComponent> script = ent->get<ScriptComponent>();
					BarrelScript* barrelScript = (BarrelScript*)sm->getScript(script->scriptId);
					barrelScript->grabBarrel();
					totalBarrels++;
				}

				if (cubeColl->colliderId == 2 && totalBarrels >= 3) {
					text1Ref->turnVisible(true);
					text2Ref->turnVisible(true);
					disableMovement = true;
				}

				if (currentPosition.y < pos.y - cubeColl->height + 0.01) {
					playerFeet.y = pos.y - cubeColl->height;
				}
				if (currentPosition.y > pos.y + cubeColl->height - 0.01) {
					playerFeet.y = pos.y + cubeColl->height;
					isFalling = false;
					verticalVelocity = 0;
				}
			}
		
			if (playerFeet.x < pos.x + cubeColl->width && playerFeet.x > pos.x - cubeColl->width &&
				playerFeet.y < pos.y + cubeColl->height && playerFeet.y > pos.y - cubeColl->height &&
				playerFeet.z < pos.z + cubeColl->length && playerFeet.z > pos.z - cubeColl->length) {


				if (currentPosition.x < pos.x - cubeColl->width + 0.01) {
					playerFeet.x = pos.x - cubeColl->width;
				}
				if (currentPosition.x > pos.x + cubeColl->width - 0.01) {
					playerFeet.x = pos.x + cubeColl->width;
				}
			}
			if (playerFeet.x < pos.x + cubeColl->width && playerFeet.x > pos.x - cubeColl->width &&
				playerFeet.y < pos.y + cubeColl->height && playerFeet.y > pos.y - cubeColl->height &&
				playerFeet.z < pos.z + cubeColl->length && playerFeet.z > pos.z - cubeColl->length) {

				if (currentPosition.z < pos.z - cubeColl->length + 0.01) {
					playerFeet.z = pos.z - cubeColl->length;
				}
				if (currentPosition.z > pos.z + cubeColl->length - 0.01) {
					playerFeet.z = pos.z + cubeColl->length;
				}
			}

		}
		else {
			if (playerHead.x < pos.x + cubeColl->width && playerHead.x > pos.x - cubeColl->width &&
				playerHead.y < pos.y + cubeColl->height && playerHead.y > pos.y - cubeColl->height &&
				playerHead.z < pos.z + cubeColl->length && playerHead.z > pos.z - cubeColl->length) {

				if (cubeColl->colliderId == 1) {
					ComponentHandle<ScriptComponent> script = ent->get<ScriptComponent>();
					BarrelScript* barrelScript = (BarrelScript*)sm->getScript(script->scriptId);
					barrelScript->grabBarrel();
					totalBarrels++;
				}

				if (currentPosition.y < pos.y - cubeColl->height + 0.01) {
					playerHead.y = pos.y - cubeColl->height;
					isFalling = false;
					verticalVelocity = 0;
				}
				if (currentPosition.y > pos.y + cubeColl->height - 0.01) {
					playerHead.y = pos.y + cubeColl->height;
				}
			}

			if (playerHead.x < pos.x + cubeColl->width && playerHead.x > pos.x - cubeColl->width &&
				playerHead.y < pos.y + cubeColl->height && playerHead.y > pos.y - cubeColl->height &&
				playerHead.z < pos.z + cubeColl->length && playerHead.z > pos.z - cubeColl->length) {


				if (currentPosition.x < pos.x - cubeColl->width + 0.01) {
					playerHead.x = pos.x - cubeColl->width;
				}
				if (currentPosition.x > pos.x + cubeColl->width - 0.01) {
					playerHead.x = pos.x + cubeColl->width;
				}
			}
			if (playerHead.x < pos.x + cubeColl->width && playerHead.x > pos.x - cubeColl->width &&
				playerHead.y < pos.y + cubeColl->height && playerHead.y > pos.y - cubeColl->height &&
				playerHead.z < pos.z + cubeColl->length && playerHead.z > pos.z - cubeColl->length) {

				if (currentPosition.z < pos.z - cubeColl->length + 0.01) {
					playerHead.z = pos.z - cubeColl->length;
				}
				if (currentPosition.z > pos.z + cubeColl->length - 0.01) {
					playerHead.z = pos.z + cubeColl->length;
				}
			}
		}
	});

	cam->position = playerFeet + glm::vec3(0, 5, 0);

	// Handles mouse inputs
	if ((glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) && !disableMovement)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::mat4 m = glm::mat4(1.0f);

		//AQUI
		glm::vec3 cam_right = glm::cross(cam->up, cam->orientation);

		m = glm::rotate(m, glm::radians(-rotX), glm::normalize(glm::cross(cam->orientation, cam->up)));

		// Rotates the Orientation left and right
		glm::mat4 m2 = glm::mat4(1.0f);

		m2 = glm::rotate(m2, glm::radians(-rotY), cam->up);
		m2 = glm::rotate(m2, glm::radians(rotX), cam_right);

		cam->orientation = m2 * glm::vec4(cam->orientation, 1.);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

}

void FirstPersonCameraScript::setGravityScriptRef(GravityScript* grs) {
	gravityRef = grs;
}

void FirstPersonCameraScript::setScriptRef(ScriptManager* sm)
{
	this->sm = sm;
}

void FirstPersonCameraScript::setTextsRef(TextScript* text1, TextScript* text2) {
	text1Ref = text1;
	text2Ref = text2;
}