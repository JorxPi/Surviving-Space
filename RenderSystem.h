#pragma once

#include<iostream>
#include<string>
#include<map>
#include "ECS.h"
#include "Components.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "MeshManager.h"

using namespace std;
using namespace ECS;

class RenderSystem : public EntitySystem
{
public:

    RenderSystem(int width, int height);
    virtual void tick(World* world, float deltaTime) override;
    void setCamera(Entity* camera);

private:
    int width;
    int height;
    Entity* camera;
    TextureManager textureManager;
    MeshManager meshManager;
    Renderer rend;

    float t = 0;

};