#include "Shader.h"

#include <iostream>
#include <glad/glad.h>

static const char* readTextFile(const char* path)
{
	FILE* file;
	fopen_s(&file, path, "rb");

	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	char* res = (char*)malloc(size + 1);
	fseek(file, 0, SEEK_SET);

	fread_s(res, size + 1, 1, size, file);

	res[size] = '\0';

	fclose(file);

	return res;
}

static unsigned int compileShader(const char* src, int type)
{
	unsigned int id = glCreateShader(type);

	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int size;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
		char* msg = (char*)malloc(size + 1);
		glGetShaderInfoLog(id, size + 1, &size, msg);
		std::cerr << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER ERROR: " << msg << std::endl;
		free(msg);
	}

	return id;
}

void Shader::Load(const char* vsPath, const char* fsPath)
{
	_program = glCreateProgram();

	const char* vSrc = readTextFile(vsPath);
	const char* fSrc = readTextFile(fsPath);
	
	unsigned int vs = compileShader(vSrc, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fSrc, GL_FRAGMENT_SHADER);

	glAttachShader(_program, vs);
	glAttachShader(_program, fs);
	glLinkProgram(_program);
	glValidateProgram(_program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	free((char*)vSrc);
	free((char*)fSrc);

	Bind();
}

Shader::Shader(const char* vsPath, const char* fsPath)
{
	Load(vsPath, fsPath);
}

Shader::~Shader(void)
{
	glDeleteProgram(_program);
}
 
void Shader::Bind() const
{
	glUseProgram(_program);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

int Shader::GetUniformLocation(const char * name) const
{
	return glGetUniformLocation(_program, name);
}

void Shader::Uniform1i(const char * name, const int val) const
{
	Bind();
	glUniform1i(GetUniformLocation(name), val);
}

void Shader::Uniform1ui(const char * name, const unsigned int val) const
{
	Bind();
	glUniform1ui(GetUniformLocation(name), val);
}

void Shader::Uniform1f(const char * name, const float val) const
{
	Bind();
	glUniform1f(GetUniformLocation(name), val);
}

void Shader::Uniform2f(const char * name, const glm::vec2 & vec) const
{
	Bind();
	glUniform2fv(GetUniformLocation(name), 1, &vec[0]);
}

void Shader::Uniform3f(const char * name, const glm::vec3 & vec) const
{
	Bind();
	glUniform3fv(GetUniformLocation(name), 1, &vec[0]);
}

void Shader::Uniform4f(const char * name, const glm::vec4 & vec) const
{
	Bind();
	glUniform4fv(GetUniformLocation(name), 1, &vec[0]);
}

void Shader::UniformMat3f(const char * name, const glm::mat3 & mat) const
{
	Bind();
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::UniformMat4f(const char * name, const glm::mat4& mat) const
{
	Bind();
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

Shader* Shader::_basicShader = nullptr;