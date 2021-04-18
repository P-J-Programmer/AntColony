#include "FileSystem/FileLoading.h"

#include <filesystem>
#include <fstream>
#include <iterator>

#include "STB/stb_image.h"
#include "Logger/Logger.h"

std::string LoadFile(const std::string& Filepath)
{
    std::ifstream InputFileStream(Filepath, std::ios::binary);
    if (!InputFileStream)
    {
        Log().Error("Failed to read:", Filepath);
        return {};
    }
    return { std::istreambuf_iterator<char>(InputFileStream), std::istreambuf_iterator<char>() };
}

std::string ExtractFileName(const std::string& Filepath)
{
    std::filesystem::path Path = Filepath;
    return Path.stem().string();
}

bool IsValidDirectory(const std::string& DirectoryPath)
{
    using namespace std::filesystem;
    path Path = current_path() / DirectoryPath;

    if (!exists(Path))
    {
        Log().Error(DirectoryPath, " does not exist.");
        return false;

    }

    if (!is_directory(DirectoryPath))
    {
        Log().Error(DirectoryPath, " is not a valid directory.");
        return false;
    }

    return true;
}

std::string DirectoryPath(const std::string& Filepath)
{

    return std::string();
}


FStbImageInfo::FStbImageInfo(const std::string& Filepath)
{
    constexpr int bAlwaysFlipOnLoad = 1;
    stbi_set_flip_vertically_on_load(bAlwaysFlipOnLoad);
    m_Data = stbi_load(Filepath.c_str(), &m_Width, &m_Height, &m_NumChannels, /*desired_channels*/0);
}

FStbImageInfo::~FStbImageInfo()
{
    stbi_image_free(m_Data);
}


std::unique_ptr<FImageInfo> LoadImage(const std::string& Filepath)
{
    return std::make_unique<FStbImageInfo>(Filepath);
}
