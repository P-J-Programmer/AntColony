#include "Renderer/TextureAtlas.h"

#include "FileSystem/FileLoading.h"
#include "Logger/Logger.h"
#include "Renderer/TextureManager.h"

namespace
{
	struct FSpriteFrame
	{
		glm::vec2 Min;
		glm::vec2 Max;
	};

	std::vector<glm::vec2> CreateTextureCoordinates(const FSpriteFrame& SpriteFrame)
	{
		constexpr unsigned int NumCoordinates = 4;
		std::vector<glm::vec2> TextureCoordinates(NumCoordinates);
		TextureCoordinates[0] = { SpriteFrame.Min.x, SpriteFrame.Min.y };
		TextureCoordinates[1] = { SpriteFrame.Max.x, SpriteFrame.Min.y };
		TextureCoordinates[2] = { SpriteFrame.Max.x, SpriteFrame.Max.y };
		TextureCoordinates[3] = { SpriteFrame.Min.x, SpriteFrame.Max.y };
		return TextureCoordinates;
	}
}

FTextureAtlas::FTextureAtlas(const FTextureManager& TextureManager, const std::string& FilePath)
{
	std::string LayoutData = LoadFile(FilePath);
	std::stringstream LayoutDataStream(LayoutData);

	ParseHeader(TextureManager, LayoutDataStream);
	ParseAtlasData(TextureManager, LayoutDataStream);

}

const FSubTexture* FTextureAtlas::GetSubTexture(const std::string& Name) const
{
	if (!IsValidSubTexture(Name))
	{
		return nullptr;
	}
	return &m_Atlas.at(Name);
}

bool FTextureAtlas::IsValidSubTexture(const std::string& Name) const
{
	return m_Atlas.find(Name) != m_Atlas.end();
}

void FTextureAtlas::ParseHeader(const FTextureManager& TextureManager, std::stringstream& OutLayoutDataStream)
{
	std::string TextureHeading;
	OutLayoutDataStream >> TextureHeading >> m_Texture;
	if (!TextureManager.IsValidTexture(m_Texture))
	{
		Log().Error("Invalid Atlas MetaData: Texture not found");
		return;
	}

	std::string SizeHeading;
	std::string WidthHeading;
	std::string HeightHeading;
	OutLayoutDataStream >> SizeHeading >> WidthHeading >> m_Width >> HeightHeading >> m_Height;
}

void FTextureAtlas::ParseAtlasData(const FTextureManager& TextureManager, std::stringstream& OutLayoutDataStream)
{
	const FTexture* SpriteSheet = TextureManager.GetTexture(m_Texture);

	while (OutLayoutDataStream)
	{
		std::string NameHeader;
		std::string Name;
		OutLayoutDataStream >> NameHeader >> Name;
		if (OutLayoutDataStream)
		{
			std::string PosXHeader;
			std::string PosYHeader;
			std::string WidthHeader;
			std::string HeightHeader;
			int X = 0;
			int Y = 0;
			int Width = 0;
			int Height = 0;
			OutLayoutDataStream >> PosXHeader >> X >> PosYHeader >> Y >> WidthHeader >> Width >> HeightHeader >> Height;
			
			if (Width == 0 || Height == 0)
			{
				Log().Error("Invalid Atlas MetaData: Width and height must be non-zero");
				return;
			}

			FSpriteFrame SpriteFrame;
			SpriteFrame.Min = { float(X) / m_Width, 1.0f - float(Y) / (float)m_Height };
			SpriteFrame.Max = { float(X + Width) / m_Width, 1.0f -float(Y + Height) / (float)m_Height };

			m_Atlas.emplace(Name, FSubTexture(*SpriteSheet, CreateTextureCoordinates(SpriteFrame))); 
		}
	}
}

