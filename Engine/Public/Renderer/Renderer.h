#pragma once

#include <memory>
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Renderer/Material.h"
#include "Renderer/ShaderManager.h"
#include "Renderer/TextureManager.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"

struct FScene
{
	glm::mat4 ViewProjection;
};

class FRenderer
{
public:
	FRenderer();

public:
	void SetScene(const std::string& Shader, const FScene& Scene);
	void SubmitDrawRequest(const FMaterial& Material, const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices);
	void Flush();

	const FTextureManager& GetTextureManager() { return m_TextureManager; }
private:
	FShaderManager m_ShaderManager;
	FTextureManager m_TextureManager;

	std::unique_ptr<FVertexBuffer> m_VertexBuffer;
	std::unique_ptr<FIndexBuffer> m_IndexBuffer;
	std::unique_ptr<FVertexArray> m_VertexArray;

};