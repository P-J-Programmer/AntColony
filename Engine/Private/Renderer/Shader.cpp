#include "Renderer/Shader.h"

#include <unordered_map>

#include <glm/gtc/type_ptr.hpp>

#include "Filesystem/FileLoading.h"
#include "Logger/Logger.h"
#include "Utility/OpenGlUtilities.h" 

namespace 
{ 
	using FShaderDefines = std::unordered_map<GLenum, const GLchar*>;
	const FShaderDefines& GetShaderDefines()
	{
		static FShaderDefines ShaderDefines =
		{
			{ GL_VERTEX_SHADER, "#version 450\n#define VERTEX_SHADER\n"},
			{ GL_FRAGMENT_SHADER, "#version 450\n#define FRAGMENT_SHADER\n"},
		};

		return ShaderDefines;
	}

	constexpr int InvalidUniformLocation = -1; // Defined by OpenGL
}

FShader::FShader(const std::string& FilePath)
{
	m_Id = glCreateProgram();
	std::string Source = LoadFile(FilePath);
	LoadShader(Source);
}

FShader::~FShader()
{
	glDeleteProgram(m_Id);
}

bool FShader::SetUniform(const std::string& Uniform, GLint Value)
{
	Bind();
	GLint Location = glGetUniformLocation(m_Id, Uniform.c_str());
	if (Location == InvalidUniformLocation)
	{
		Log().Error("Invalid Uniform:", Uniform);
		return false;
	}
	glUniform1i(Location, Value);
	return true;
}

bool FShader::SetUniform(const std::string& Uniform, const glm::mat4& Value)
{
	Bind();
	GLint Location = glGetUniformLocation(m_Id, Uniform.c_str());
	if (Location == InvalidUniformLocation)
	{
		Log().Error("Invalid Uniform:", Uniform);
		return false;
	}
	glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(Value));
	return true;
}

void FShader::Bind() const
{
	glUseProgram(m_Id);
}

void FShader::Unbind()
{
	glUseProgram(0);
}

void FShader::LoadShader(const std::string& Source)
{
	for (const auto& [ShaderType, Defines] : GetShaderDefines())
	{
		const GLchar* ShaderSource[] = { Defines, Source.c_str() };
		const GLuint NumSourceStrings = sizeof(ShaderSource) / sizeof(ShaderSource[0]);

		GLuint ShaderId = glCreateShader(ShaderType);
		glShaderSource(ShaderId, NumSourceStrings, ShaderSource, /*length*/ nullptr);
		glCompileShader(ShaderId);

		if (OpenGlUtilities::CheckShaderCompileStatus(ShaderId))
		{
			glAttachShader(m_Id, ShaderId);
		}
		else
		{
			glDeleteShader(m_Id);
			return;
		}
	}

	glLinkProgram(m_Id);

	if (!OpenGlUtilities::CheckProgramLinkStatus(m_Id))
	{
		glDeleteShader(m_Id);
		Log().Error("Failed to link shader");
	}
}
