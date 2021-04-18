#pragma once

#include "glad/glad.h"

class FImageInfo;

namespace OpenGlUtilities
{
	bool InitGlad();
	GLenum ImageFormat(const FImageInfo* const ImageInfo);
	void OnOpenGlError(
		GLenum Source​,
		GLenum Type​,
		GLuint Id​,
		GLenum Severity​,
		GLsizei Length​,
		const GLchar* Message​,
		const void* UserParam​);

	bool CheckShaderCompileStatus(const GLuint ShaderId);
	bool CheckProgramLinkStatus(const GLuint ProgramId);
}