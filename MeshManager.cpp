#include "MeshManager.h"

Mesh MeshManager::GetMesh(const char* filepath)
{

	auto pos = meshes.find(filepath);
	if (pos == meshes.end()) {

		Mesh mesh = Mesh(filepath);
		meshes.insert(std::pair<string, Mesh>(filepath, mesh));

		return mesh;

	}
	else {

		return pos->second;

	}
}

void MeshManager::Delete() {

	map<string, Mesh>::iterator it;

	for (it = meshes.begin(); it != meshes.end(); it++)
	{
		it->second.Delete();
	}

	delete this;

}
