#include "ScriptManager.h"

ScriptManager::ScriptManager() {

}

int ScriptManager::AddScript(Script* script) {

    scripts.insert(std::pair<int, Script*>(autoinc, script));

    autoinc += 1;

    return scripts.size() - 1;

    
}

void ScriptManager::tickScript(int scriptId, float deltaTime) {


    scripts[scriptId]->tickScript(deltaTime);
}

Script* ScriptManager::getScript(int scriptId) {
    return scripts[scriptId];
}

void ScriptManager::Delete() {

    map<int, Script*>::iterator it;

    for (it = scripts.begin(); it != scripts.end(); it++)
    {
        it->second->Delete();
    }

    delete this;

}


