#pragma once

#include<iostream>
#include<string>
#include<map>
#include "ECS.h"
#include "Components.h"
#include "Script.h"

using namespace std;
using namespace ECS;

class ScriptManager
{
public:

    ScriptManager();

    virtual int AddScript(Script* script);
    virtual void tickScript(int scriptId, float deltaTime);
    Script* getScript(int scriptId);

    void Delete();

private:
    map<int, Script*> scripts;

    int autoinc = 0;

};