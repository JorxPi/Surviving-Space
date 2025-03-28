#include "TextureManager.h"

Texture TextureManager::GetTexture(const char* filepath)
{
	
	auto pos = textures.find(filepath);
	if (pos == textures.end()) {
		
		Texture texture = Texture(filepath, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		textures.insert(std::pair<string, Texture>(filepath, texture));

		return texture;

	}
	else {
		
		return pos->second;

	}
}

Texture TextureManager::GetCubemapTexture(const char* filepath)
{

	auto pos = textures.find(filepath);
	if (pos == textures.end()) {

		Texture texture = Texture(filepath, GL_TEXTURE_CUBE_MAP, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		textures.insert(std::pair<string, Texture>(filepath, texture));

		return texture;

	}
	else {

		return pos->second;

	}
}

void TextureManager::Delete() {

	map<string, Texture>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++)
	{
		it->second.Delete();
	}

	delete this;

}
