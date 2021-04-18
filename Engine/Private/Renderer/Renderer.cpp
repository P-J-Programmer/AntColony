#include "Renderer/Renderer.h"

#include "Logger/Logger.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

FRenderer::FRenderer()
{
    FBufferLayout Layout;
    Layout.AddVertexAttrib<Vertex4D>("Position");
    Layout.AddVertexAttrib<Vertex2D>("Texture");
    m_VertexBuffer = std::make_unique<FVertexBuffer>(Layout);
    m_IndexBuffer = std::make_unique<FIndexBuffer>();
    m_VertexArray = std::make_unique<FVertexArray>(*m_VertexBuffer, *m_IndexBuffer);
}

void FRenderer::SetScene(const std::string& ShaderName, const FScene& Scene)
{
    const FShader* Shader = m_ShaderManager.GetShader(ShaderName);
    if (!Shader)
    {
        Log().Error("Failed to find shader: ", ShaderName);
        return;
    }

    Shader->Bind();
    m_ShaderManager.SetUniform(ShaderName, "u_ViewProjection", Scene.ViewProjection);
}

void FRenderer::SubmitDrawRequest(const FMaterial& Material, const std::vector<float>& Vertices, const std::vector<unsigned int>& Indices)
{
    for (const FTextureUnit& TextureUnit : Material.Textures)
    {
        m_ShaderManager.SetUniform(Material.Shader, TextureUnit.Uniform, TextureUnit.Slot);

        const FTexture* Texture = m_TextureManager.GetTexture(TextureUnit.Asset);
        if (!Texture)
        {
            Log().Error("Failed to retreive texture:", TextureUnit.Asset);
            continue;
        }

        Texture->Bind(TextureUnit.Slot);
    }

    m_VertexBuffer->BufferData(Vertices);
    m_IndexBuffer->BufferIndices(Indices);

    Flush();
}

void FRenderer::Flush()
{
    m_VertexArray->Bind();
    glDrawElements(GL_TRIANGLES, m_IndexBuffer->Count(), GL_UNSIGNED_INT, /* indices */ 0);
}
