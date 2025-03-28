#pragma once

#include <map>
#include <string>

#include "Mesh.h"

using namespace std;

class MeshManager
{
public:

    Mesh GetMesh(const char* filepath);

    void Delete();

private:
    map<string, Mesh> meshes;

};