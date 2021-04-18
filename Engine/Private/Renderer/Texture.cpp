#include "Renderer/Texture.h"

#include "FileSystem/FileLoading.h"

#include "Utility/OpenGlUtilities.h"

FTexture::FTexture(const std::string& FilePath)
{
	std::unique_ptr<FImageInfo> ImageInfo = LoadImage(FilePath);

	glCreateTextures(GL_TEXTURE_2D, /*NumTextures*/ 1, &m_Id);

	constexpr GLint NumMipMaps = 1;
	const GLenum Format = OpenGlUtilities::ImageFormat(ImageInfo.get());
	const GLsizei Width = ImageInfo->GetWidth();
	const GLsizei Height = ImageInfo->GetHeight();

	glTextureStorage2D(m_Id, NumMipMaps, GL_RGBA8, Width, Height);

	glTextureParameteri(m_Id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_Id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureParameteri(m_Id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_Id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTextureSubImage2D(m_Id, 0, 0, 0, Width, Height, Format, GL_UNSIGNED_BYTE, ImageInfo->GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

FTexture::~FTexture()
{
	glDeleteTextures(1, &m_Id);
}

void FTexture::Bind(GLuint Unit) const
{
	glBindTextureUnit(Unit, m_Id);
}
