#include "SparkPCH.h"
#include "OpenGLTexture.h"

#include "Spark/Platform/OpenGL/OpenGL.h"
#include <stb_image.h>

namespace Spark
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
		: m_Path(filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		SPARK_CORE_ASSERT(data, "Failed to load image");
		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glCheckError();
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);
		glCheckError();
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glCheckError();
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glCheckError();
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, (channels == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		glCheckError();
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
		glCheckError();
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
		glCheckError();
	}
}