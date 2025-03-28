#pragma once

#include "Script.h"

using namespace std;

class SkyboxScript : public Script
{

    using Script::Script;

public:

    SkyboxScript(GLFWwindow* window, World* world, Entity* entity, Entity* camera);

    void startScript() override;

    void tickScript(float deltaTime) override;

private:

    Entity* camera;

};