#pragma once

#include <string>
#include <vector>

struct FTextureUnit
{
	std::string Asset;
	std::string Uniform;
	unsigned int Slot = 0;
};

struct FMaterial
{
	std::string Shader;
	std::vector<FTextureUnit> Textures;
};