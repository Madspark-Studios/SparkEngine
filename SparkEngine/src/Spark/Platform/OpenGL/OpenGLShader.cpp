#include "SparkPCH.h"
#include "OpenGLShader.h"

#include "OpenGL.h"
#include <glm/gtc/type_ptr.hpp>

namespace Spark
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex") 
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") 
			return GL_FRAGMENT_SHADER;

		SPARK_CORE_ASSERT(false, "Unkown shader type");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shader_sources = PreProcess(source);
		Compile(shader_sources);
	}

	Spark::OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragSrc)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragSrc;
		Compile(sources);
	}

	Spark::OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Spark::OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Spark::OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniform1i(location, value);
		glCheckError();
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniform1f(location, value);
		glCheckError();
	}

	void OpenGLShader::SetUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniform2fv(location, 1, glm::value_ptr(value));
		glCheckError();
	}

	void OpenGLShader::SetUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniform3fv(location, 1, glm::value_ptr(value));
		glCheckError();
	}

	void OpenGLShader::SetUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniform4fv(location, 1, glm::value_ptr(value));
		glCheckError();
	}

	void OpenGLShader::SetUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		glCheckError();
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glCheckError();
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
		glCheckError();
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			SPARK_CORE_ASSERT(false, "Could not open file:{0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shader_sources;

		const char* type_token = "#type";
		size_t type_token_length = strlen(type_token);
		size_t pos = source.find(type_token, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			SPARK_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + type_token_length + 1;
			std::string type = source.substr(begin, eol - begin);
			SPARK_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t next_line_pos = source.find_first_not_of("\r\n", eol);
			pos = source.find(type_token, next_line_pos);
			shader_sources[ShaderTypeFromString(type)] = source.substr(next_line_pos, pos - (next_line_pos == std::string::npos ? source.size() - 1 : next_line_pos));
		}
		return shader_sources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shader_sources)
	{
		GLuint program = glCreateProgram();
		glCheckError();
		std::vector<GLenum> gl_shaders;
		for (auto& kv : shader_sources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);
			glCheckError();

			const GLchar* source_c_str = source.c_str();
			glShaderSource(shader, 1, &source_c_str, 0);
			glCheckError();

			glCompileShader(shader);
			glCheckError();

			GLint is_compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
			glCheckError();
			if (is_compiled == GL_FALSE)
			{
				GLint max_length = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
				glCheckError();
				std::vector<GLchar> info_log(max_length);
				glGetShaderInfoLog(shader, max_length, &max_length, &info_log[0]);
				glCheckError();
				glDeleteShader(shader);
				glCheckError();
				SPARK_CORE_ERROR("{0}", info_log.data());
				SPARK_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}
			glAttachShader(program, shader);
			glCheckError();
			gl_shaders.push_back(shader);
		}
		m_RendererID = program;

		glLinkProgram(program);
		glCheckError();

		GLint is_linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked);
		if (is_linked == GL_FALSE)
		{
			GLint max_length = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
			glCheckError();
			
			std::vector<GLchar> info_log(max_length);
			glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]);
			glCheckError();
			
			glDeleteProgram(program);
			glCheckError();

			for (auto id : gl_shaders)
			{
				glDeleteShader(id);
				glCheckError();
			}

			SPARK_CORE_ERROR("{0}", info_log.data());
			SPARK_CORE_ASSERT(false, "OpenGL Shader Link Failed: ");
			return;
		}

		for (auto id : gl_shaders)
		{
			glDetachShader(program, id);
			glCheckError();
		}
	}
}
