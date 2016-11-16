//=============================================================================
// ■ shaders.cpp
//-----------------------------------------------------------------------------
//   VMDE中与着色器有关的全局变量和类都在这里。
//=============================================================================

#include "global.hpp"

GLchar* temp_vertexShaderSource = NULL;
GLchar* temp_fragmentShaderSource = NULL;

Shaders::Shaders(const GLchar* vsh_src_ptr, const GLchar* fsh_src_ptr) {
	const GLchar* basic_2D_vsh_src = vsh_src_ptr;
	GLint success;
	GLchar info_log[512];

	this->basic_2D_vsh = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(this->basic_2D_vsh, 1, &basic_2D_vsh_src, NULL);
	glCompileShader(this->basic_2D_vsh);
	glGetShaderiv(this->basic_2D_vsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(this->basic_2D_vsh, 512, NULL, info_log);
		log("VSH compilation failed:\n%s", info_log);
		log("Shaders error");
	}

	// Fragment shader
	const GLchar* basic_2D_fsh_src = fsh_src_ptr;

	this->basic_2D_fsh = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(this->basic_2D_fsh, 1, &basic_2D_fsh_src, NULL);
	glCompileShader(this->basic_2D_fsh);
	glGetShaderiv(this->basic_2D_fsh, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(this->basic_2D_fsh, 512, NULL, info_log);
		log("FSH compilation failed:\n%s", info_log);
		log("Shaders error");
	}

	glGenBuffers(1, &UBO_matrix);
}

void Shaders::link_program() {
	// Link shaders
	GLint success;
	GLchar info_log[512];

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, this->basic_2D_vsh);
	glAttachShader(shaderProgram, this->basic_2D_fsh);
	glLinkProgram(shaderProgram);
	// Check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, info_log);
		log("shaders linking failed:\n%s", info_log);
		log("Shaders error");
	}
	glDeleteShader(this->basic_2D_vsh);
	glDeleteShader(this->basic_2D_fsh);
}

void Shaders::use() {
	glUseProgram(this->shaderProgram);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO_matrix);
}

void Shaders::ProjectionView(glm::mat4 projection, glm::mat4 view) {
	GLuint loc = glGetUniformLocation(shaderProgram, "View");
	GLfloat* ptrV = glm::value_ptr(view);
	glUniformMatrix4fv(loc, 1, GL_FALSE, ptrV);

	loc = glGetUniformLocation(shaderProgram, "Projection");
	GLfloat* ptrP = glm::value_ptr(projection);
	glUniformMatrix4fv(loc, 1, GL_FALSE, ptrP);
}
