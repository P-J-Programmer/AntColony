#include "OpenGlUtilities.h"

#include <vector>

#include "GLFW/glfw3.h"

#include "Logger/Logger.h"
#include "Filesystem/FileLoading.h"

namespace OpenGlUtilities
{
	void OnOpenGlError(GLenum Source​, GLenum Type​, GLuint Id​,
		GLenum Severity​, GLsizei Length​, const GLchar* Message​, const void* UserParam​)
	{
		if (Severity​ == GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			return; // OpenGL sends a lot of spam at notification level. Just turn off for now.
		}

		Log().Error(Message​);
	}

	GLenum ImageFormat(const FImageInfo* const ImageInfo)
	{
		if (!ImageInfo)
		{
			return GL_NONE;
		}

		constexpr int RGB = 3;
		constexpr int RGBA = 4;

		switch (ImageInfo->GetChannels())
		{
		case RGB:
			return GL_RGB;
		case RGBA:
			return GL_RGBA;
		default:
			return GL_NONE;
		}
	}

	bool InitGlad()
	{
		const bool GladInitSuccess = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (GladInitSuccess)
		{
			Log().Info("Initalised GLAD successfully V", GLVersion.major, ".", GLVersion.minor);
			glDebugMessageCallback(OnOpenGlError, /*UserParam*/nullptr);
		}
		else
		{
			Log().Error("Failed to initise GLAD");
		}

		return GladInitSuccess;
	}

	bool CheckShaderCompileStatus(const GLuint ShaderId)
	{
		GLint bHasCompiled = 0;
		glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &bHasCompiled);
		if (bHasCompiled == GL_FALSE)
		{
			GLint InfoLogLength = 0;
			glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);

			std::vector<GLchar> InfoLogMessage;
			InfoLogMessage.reserve(InfoLogLength);
			glGetShaderInfoLog(ShaderId, InfoLogLength, &InfoLogLength, InfoLogMessage.data());
			if (InfoLogMessage.size())
			{
				Log().Error("Shader compilation failure:", InfoLogMessage.data());
			}
		}

		return bHasCompiled == GL_TRUE;
	}

	bool CheckProgramLinkStatus(const GLuint ProgramId)
	{
		GLint bHasLinked = 0;
		glGetProgramiv(ProgramId, GL_LINK_STATUS, &bHasLinked);
		if (bHasLinked == GL_FALSE)
		{
			GLint InfoLogLength = 0;
			glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<GLchar> InfoLogMessage;
			InfoLogMessage.reserve(InfoLogLength);
			glGetProgramInfoLog(ProgramId, InfoLogLength, &InfoLogLength, InfoLogMessage.data());

			if (InfoLogMessage.size())
			{
				Log().Error("Program link failure:", InfoLogMessage.data());
			}
		}

		return bHasLinked == GL_TRUE;
	}
}