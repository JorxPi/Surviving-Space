#pragma once

#include <glm/glm.hpp>
#include "Texture.h"

struct Transform2D
{
    Transform2D(glm::vec2 position, float rotation, float scale) {
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
    }

    glm::vec2 position;
    float rotation;
    float scale;
};

struct Transform3D
{
    Transform3D(glm::vec3 position, float xrotation, float yrotation, float scale) {
        this->position = position;
        this->xrotation = xrotation;
        this->yrotation = yrotation;
        this->scale = scale;
    }

    glm::vec3 position;
    float xrotation;
    float yrotation;
    float scale;
};

struct Camera
{
    Camera(glm::vec3 position, glm::vec3 orientation, glm::vec3 up) {
        this->position = position;
        this->orientation = orientation;
        this->up = up;
    }

    glm::vec3 position;
    glm::vec3 orientation;
    glm::vec3 up;
};

struct Sprite
{
    Sprite(const char* filepath, glm::vec3 color, bool autoSize = true, glm::vec2 size = glm::vec2(1, 1), const char* shaderName = "default", bool visible = true) {
        this->filepath = filepath;
        this->color = color;
        this->autoSize = autoSize;
        this->size = size;
        this->shaderName = shaderName;
        this->visible = visible;
    }

    const char* filepath;
    glm::vec3 color;
    bool autoSize;
    glm::vec2 size = glm::vec2(1, 1);
    const char* shaderName;
    bool visible;
};

struct MeshComponent
{
    MeshComponent(const char* textureFilepath, const char* meshFilepath, const char* shaderName = "default", const char* normalsFilepath = "Textures/flat_normal.png") {
        this->textureFilepath = textureFilepath;
        this->meshFilepath = meshFilepath;
        this->shaderName = shaderName;
        this->normalsFilepath = normalsFilepath;
    }
    const char* textureFilepath;
    const char* normalsFilepath;
    const char* meshFilepath;
    const char* shaderName;
};

struct Skybox
{
    Skybox(const char* textureFilepath, const char* meshFilepath) {
        this->textureFilepath = textureFilepath;
        this->meshFilepath = meshFilepath;
    }
    const char* textureFilepath;
    const char* meshFilepath;
};

struct BoxCollider
{
    BoxCollider(float w, float h) {
        this->width = w;
        this->height = h;
    }

    float width = 0;
    float height = 0;
    bool collidedWith = false;
};

struct CubeCollider
{
    CubeCollider(float w, float h, float l, int colId = 0) {
        this->width = w;
        this->height = h;
        this->length = l;
        this->colliderId = colId;
    }

    float width = 0;
    float height = 0;
    float length = 0;
    int colliderId = 0;
};

struct ScriptComponent
{
    ScriptComponent(int scriptId) {
        this->scriptId = scriptId;
    }

    int scriptId = -1;
};