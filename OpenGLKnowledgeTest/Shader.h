#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>

class Shader {
public:
	unsigned int id;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	inline void use() { glUseProgram(id); }

	//inline void setUniformBool(const char* name, bool value)	{ glUniform1i(glGetUniformLocation(id, name), (int)value); }
	//inline void setUniformInt(const char* name, int value)		{ glUniform1i(glGetUniformLocation(id, name), value); }
	//inline void setUniformFloat(const char* name, float value)	{ glUniform1f(glGetUniformLocation(id, name), value); }
	void setUniformBool(const char* name, bool value);
	void setUniformInt(const char* name, int value);
	void setUniformFloat(const char* name, float value);

private:
	void checkShaderComp(GLuint shader, const char* type);
	void checkProgramLink(GLuint program, const char* type);
};