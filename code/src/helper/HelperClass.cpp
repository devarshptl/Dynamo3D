#include "HelperClass.h"

#include "../lib/features/MacroClass.h"

#include <iostream>
#include <fstream>
#include <streambuf>

/* [WIREFRAME SHADER FILES]
* wireframe.vert
* wireframe.frag
*/
std::string WireframeShaders[] = {
	"shader/wireframe.vert", // wireframe.vert
	"shader/wireframe.frag" // wireframe.frag
};

/* [FLAT SHADER FILES]
* flatshading.vert
* flatshading.frag
* flatshading.geom
*/
std::string FlatShaders[] = {
	"shader/flatshading.vert", // flatshading.vert
	"shader/flatshading.frag", // flatshading.frag
	"shader/flatshading.geom" // flatshading.geom
};

/* [PHONG SHADER FILES]
* phongshading.vert
* phongshading.frag
*/
std::string PhongShaders[] = {
	"shader/phongshading.vert", // phongshading.vert
	"shader/phongshading.frag" // phongshading.frag
};

/* [SHADOW SHADER FILES]
* shadow.vert
* shadow.frag
* shadow.geom
*/
std::string ShadowShaders[] = {
	"shader/shadow.vert", // shadow.vert
	"shader/shadow.frag", // shadow.frag
	"shader/shadow.geom" // shadow.geom
};

/* [SKYBOX SHADER FILES]
* skybox.vert
* skybox.frag
*/
std::string SkyBoxShaders[] = {
	"shader/skybox.vert", // skybox.vert
	"shader/skybox.frag" // skybox.frag
};

void VertexArrayObject::init()
{
	glGenVertexArrays(1, &id);
	check_gl_error();
}

void VertexArrayObject::bind()
{
	glBindVertexArray(id);
	check_gl_error();
}

void VertexArrayObject::free()
{
	glDeleteVertexArrays(1, &id);
	check_gl_error();
}

void BufferObject::init()
{
	glGenBuffers(1, &id);
	check_gl_error();
}

void BufferObject::free()
{
	glDeleteBuffers(1, &id);
	check_gl_error();
}

void VertexBufferObject::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	check_gl_error();
}

void ElementBufferObject::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	check_gl_error();
}

void Texture::init() {
	glGenTextures(1, &id);
	check_gl_error();
}

void Texture::bind(GLenum target) {
	glBindTexture(target, id);
}

void Texture::free() {
	glDeleteTextures(1, &id);
	check_gl_error();
}

void FrameBufferObject::init() {
	glGenFramebuffers(1, &id);
	check_gl_error();
}

void FrameBufferObject::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	check_gl_error();
}

void FrameBufferObject::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	check_gl_error();
}

void FrameBufferObject::free() {
	glDeleteFramebuffers(1, &id);
	check_gl_error();
}

void FrameBufferObject::check() {
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "FRAME BUFFER STATUS WRONG " << glCheckFramebufferStatus(GL_FRAMEBUFFER) << std::endl;;
		exit(0);
	}
}

void FrameBufferObject::attach_depth_texture(Texture& texture) {
	bind();
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture.id, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	unbind();
	check_gl_error();
}

void FrameBufferObject::attach_color_texture(Texture& texture) {
	bind();
	for (unsigned int i = 0; i < 6; i++) {
		GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, texture.id, 0);
	}
	unbind();
	check_gl_error();
}

bool Program::init(
	const std::string& vertex_shader_string,
	const std::string& fragment_shader_string,
	const std::string& geometry_shader_string,
	const std::string& fragment_data_name)
{
	using namespace std;
	vertex_shader = create_shader_helper(GL_VERTEX_SHADER, vertex_shader_string);
	fragment_shader = create_shader_helper(GL_FRAGMENT_SHADER, fragment_shader_string);
	geometry_shader = create_shader_helper(GL_GEOMETRY_SHADER, geometry_shader_string);

	if (!vertex_shader || !fragment_shader)
		return false;

	program_shader = glCreateProgram();

	glAttachShader(program_shader, vertex_shader);
	glAttachShader(program_shader, fragment_shader);
	if (geometry_shader != 0) {
		glAttachShader(program_shader, geometry_shader);
	}

	if (!fragment_data_name.empty()) {
		glBindFragDataLocation(program_shader, 0, fragment_data_name.c_str());
	}
	glLinkProgram(program_shader);

	GLint status;
	glGetProgramiv(program_shader, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		glGetProgramInfoLog(program_shader, 512, NULL, buffer);
		cerr << "Linker error: " << endl << buffer << endl;
		program_shader = 0;
		return false;
	}

	check_gl_error();
	return true;
}

void Program::bind()
{
	glUseProgram(program_shader);
	check_gl_error();
}

GLint Program::attrib(const std::string& name) const
{
	return glGetAttribLocation(program_shader, name.c_str());
}

GLint Program::uniform(const std::string& name) const
{
	return glGetUniformLocation(program_shader, name.c_str());
}

GLint Program::bindVertexAttribArray(
	const std::string& name, VertexBufferObject& VBO) const
{
	GLint id = attrib(name);
	if (id < 0)
		return id;
	if (VBO.id == 0)
	{
		glDisableVertexAttribArray(id);
		return id;
	}
	VBO.bind();
	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id, VBO.rows, GL_FLOAT, GL_FALSE, 0, 0);
	check_gl_error();

	return id;
}

void Program::free()
{
	if (program_shader)
	{
		glDeleteProgram(program_shader);
		program_shader = 0;
	}
	if (vertex_shader)
	{
		glDeleteShader(vertex_shader);
		vertex_shader = 0;
	}
	if (fragment_shader)
	{
		glDeleteShader(fragment_shader);
		fragment_shader = 0;
	}
	check_gl_error();
}

GLuint Program::create_shader_helper(GLint type, const std::string& shader_string)
{
	using namespace std;
	if (shader_string.empty())
		return (GLuint)0;

	GLuint id = glCreateShader(type);
	const char* shader_string_const = shader_string.c_str();
	glShaderSource(id, 1, &shader_string_const, NULL);
	glCompileShader(id);

	GLint status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		char buffer[512];
		if (type == GL_VERTEX_SHADER)
			cerr << "Vertex shader:" << endl;
		else if (type == GL_FRAGMENT_SHADER)
			cerr << "Fragment shader:" << endl;
		else if (type == GL_GEOMETRY_SHADER)
			cerr << "Geometry shader:" << endl;
		cerr << shader_string << endl << endl;
		glGetShaderInfoLog(id, 512, NULL, buffer);
		cerr << "Error: " << endl << buffer << endl;
		return (GLuint)0;
	}
	check_gl_error();

	return id;
}

Program ProgramFactory::createWireframeShader(const std::string& fragment_data_name) {
	Program program;
	std::string vertex_shader = readShader(WireframeShaders[0]);
	std::string fragment_shader = readShader(WireframeShaders[1]);
	std::string geometry_shader;
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	return program;
}

Program ProgramFactory::createFlatShader(const std::string& fragment_data_name) {
	Program program;
	std::string vertex_shader = readShader(FlatShaders[0]);
	std::string fragment_shader = readShader(FlatShaders[1]);
	std::string geometry_shader = readShader(FlatShaders[2]);
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	return program;
}

Program ProgramFactory::createPhongShader(const std::string& fragment_data_name) {
	Program program;
	std::string vertex_shader = readShader(PhongShaders[0]);
	std::string fragment_shader = readShader(PhongShaders[1]);
	std::string geometry_shader;
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	return program;
}

Program ProgramFactory::createShadowShader(const std::string& fragment_data_name) {
	Program program;
	std::string vertex_shader = readShader(ShadowShaders[0]);
	std::string fragment_shader = readShader(ShadowShaders[1]);
	std::string geometry_shader = readShader(ShadowShaders[2]);
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	return program;
}

Program ProgramFactory::createSkyboxShader(const std::string& fragment_data_name) {
	Program program;
	std::string vertex_shader = readShader(SkyBoxShaders[0]);
	std::string fragment_shader = readShader(SkyBoxShaders[1]);
	std::string geometry_shader;
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	program.init(vertex_shader.data(), fragment_shader.data(), geometry_shader.data(), fragment_data_name);
	return program;
}

std::string ProgramFactory::readShader(const std::string& path) {
	std::ifstream infile(path, std::ios::binary);
	ASSERT(infile.is_open(), std::string("Shader file not exists: ") + path);
	return std::string((std::istreambuf_iterator<char>(infile)),
		std::istreambuf_iterator<char>());
}

void _check_gl_error(const char* file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}