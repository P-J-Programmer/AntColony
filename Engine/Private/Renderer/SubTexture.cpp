#include "Renderer/SubTexture.h"

FSubTexture::FSubTexture(const FTexture& SpriteSheet, const FTextureCoordinates& TextureCoordinates) :
	m_SpriteSheet(SpriteSheet),
	m_TextureCoordinates(TextureCoordinates)
{}

