#pragma once

#include "Script.h"
#include "GravityScript.h"
#include "BarrelScript.h"
#include "ScriptManager.h"
#include "TextScript.h"

using namespace std;

class FirstPersonCameraScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime) override;

    void setGravityScriptRef(GravityScript* grs);
    void setScriptRef(ScriptManager* sm);
    void setTextsRef(TextScript* text1, TextScript* text2);

private:

    glm::vec3 direction = glm::vec3(1., 0., 0.);

    bool firstClick = true;
    float speed = 0.015f;
    float verticalVelocity = 0.0f;
    float sensitivity = 100.0f;
    float gravity = 0.002f;
    bool isFalling = false;
    bool gravityDirection = true;
    int totalBarrels = 0;
    bool disableMovement = false;

    GravityScript* gravityRef;

    ScriptManager* sm;

    TextScript* text1Ref;
    TextScript* text2Ref;
};