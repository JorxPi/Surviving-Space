#include "ScriptSystem.h"

ScriptSystem::ScriptSystem() {
}

void ScriptSystem::tick(World* world, float deltaTime)
{
    world->each<ScriptComponent>([&](Entity* ent, ComponentHandle<ScriptComponent> script) {

        scriptManager.tickScript(script->scriptId, deltaTime);

    });
}

ScriptManager* ScriptSystem::getScriptManager() {

    return &scriptManager;

}