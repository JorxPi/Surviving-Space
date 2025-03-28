#pragma once

#include "Script.h"

class SpawnerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime) override;

private:

    float delay = 5000.f;
    float t = 0.;
    int counter = 0;

};
