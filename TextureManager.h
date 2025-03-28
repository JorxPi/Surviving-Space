#pragma once

#include <map>
#include <string>

#include "Texture.h"

using namespace std;

class TextureManager
{
public:

    Texture GetTexture(const char* filepath);

    Texture GetCubemapTexture(const char* filepath);

    void Delete();

private:
    map<string, Texture> textures;

};