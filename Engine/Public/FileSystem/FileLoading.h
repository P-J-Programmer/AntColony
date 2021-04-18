#pragma once

#include <memory>
#include <string>

std::string LoadFile(const std::string& Filepath);

/*
 * Returns file name stripped of extension.
 */
std::string ExtractFileName(const std::string& Filepath);
bool IsValidDirectory(const std::string& Path);

class FImageInfo
{
public:
	FImageInfo() = default;
	virtual ~FImageInfo() {};

public:
	virtual const unsigned char* GetData() const = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetChannels() const = 0;

private:
	FImageInfo(const FImageInfo& Buffer) = delete;
	FImageInfo& operator=(const FImageInfo& Buffer) = delete;

	FImageInfo(FImageInfo&& Buffer) = delete;
	FImageInfo& operator=(FImageInfo&& Buffer) = delete;
};

class FStbImageInfo : public FImageInfo
{
public:
	FStbImageInfo(const std::string& FilePath);
    virtual ~FStbImageInfo() override;

public:
    virtual const unsigned char* GetData() const override { return m_Data; }
    virtual int GetWidth() const override { return m_Width; }
    virtual int GetHeight() const override { return m_Height; }
    virtual int GetChannels() const override { return m_NumChannels; }

private:
    unsigned char* m_Data = nullptr;
    int m_Width = 0;
    int m_Height = 0;
    int m_NumChannels = 0;
};

std::unique_ptr<FImageInfo> LoadImage(const std::string& Filepath);