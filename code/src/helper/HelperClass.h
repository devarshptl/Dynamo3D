#ifndef __HELPERS_H__
#define __HELPERS_H__

#include <string>
#include <vector>
#include <glm/glm.hpp>  // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <iostream>

#ifdef _WIN32
#  include <windows.h>
#  undef max
#  undef min
#  undef DrawText
#endif

#ifndef __APPLE__
#  define GLEW_STATIC
#  include <GL/glew.h>
#endif

#ifdef __APPLE__
#   include <OpenGL/gl3.h>
#   define __gl_h_ /* Prevent inclusion of the old gl.h */
#else
#   ifdef _WIN32
#       include <windows.h>
#   endif
#   include <GL/gl.h>
#endif

// From: https://blog.nobel-joergensen.com/2013/01/29/debugging-opengl-using-glgeterror/
void _check_gl_error(const char* file, int line);

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

class VertexArrayObject
{
public:
	unsigned int id;

	VertexArrayObject() : id(0) {}

	// Create a new VAO
	void init();

	// Select this VAO for subsequent draw calls
	void bind();

	// Release the id
	void free();
};

class BufferObject {
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	BufferObject(GLuint _id) : id{ _id }, rows{ 0 }, cols{ 0 } {}

	// Create a new empty BufferObject
	virtual void init();
	// Select this BufferObject for subsequent draw calls
	virtual void bind() = 0;
	// Release the id
	virtual void free();
	// Updates the BO
	template<typename T>
	void update(const std::vector<T>& array) {
		assert(id != 0);
		assert(!array.empty());
		update_helper(sizeof(T), array.size(), (const void*)array.data());
		cols = array.size();
		rows = 1;
		check_gl_error();
	};

	void update(const std::vector<glm::vec2>& array) {
		assert(id != 0);
		assert(!array.empty());
		update_helper(sizeof(glm::vec2), array.size(), (const void*)array.data());
		cols = array.size();
		rows = array[0].length();
		check_gl_error();
	};
	void update(const std::vector<glm::vec3>& array) {
		assert(id != 0);
		assert(!array.empty());
		update_helper(sizeof(glm::vec3), array.size(), (const void*)array.data());
		cols = array.size();
		rows = array[0].length();
		check_gl_error();
	};
private:
	virtual void update_helper(size_t size_of_t, size_t array_size, const void* data) = 0;
public:
	GLuint id;
	GLuint rows;
	GLuint cols;
};

class VertexBufferObject : public BufferObject {
public:
	VertexBufferObject() : BufferObject(0) {}
	void bind() override;
private:
	void update_helper(size_t size_of_t, size_t array_size, const void* data) override {
		glBindBuffer(GL_ARRAY_BUFFER, id);
		glBufferData(GL_ARRAY_BUFFER, size_of_t * array_size, data, GL_DYNAMIC_DRAW);
	};
};

class ElementBufferObject : public BufferObject {
public:
	ElementBufferObject() : BufferObject(0) {}
	void bind() override;
private:
	void update_helper(size_t size_of_t, size_t array_size, const void* data) override {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_t * array_size, data, GL_DYNAMIC_DRAW);
	};
};

class Texture {
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	Texture() : id{ 0 } {}
	void init();
	void bind(GLenum target);
	void free();

	GLuint id;
};

class FrameBufferObject {
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	FrameBufferObject() : id{ 0 } {}
	void init();
	void bind();
	void unbind();
	void free();
	void check();
	void attach_depth_texture(Texture& texture);
	void attach_color_texture(Texture& texture);
private:
	GLuint id;
};

class Program
{
public:
	typedef unsigned int GLuint;
	typedef int GLint;

	GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint program_shader;
	GLuint geometry_shader;

	Program() : vertex_shader(0), fragment_shader(0), program_shader(0) { }

	// Create a new shader from the specified source strings
	bool init(const std::string& vertex_shader_string,
		const std::string& fragment_shader_string,
		const std::string& geometry_shader_string,
		const std::string& fragment_data_name);

	// Select this shader for subsequent draw calls
	void bind();

	// Release all OpenGL objects
	void free();

	// Return the OpenGL handle of a named shader attribute (-1 if it does not exist)
	GLint attrib(const std::string& name) const;

	// Return the OpenGL handle of a uniform attribute (-1 if it does not exist)
	GLint uniform(const std::string& name) const;

	// Bind a per-vertex array attribute
	GLint bindVertexAttribArray(const std::string& name, VertexBufferObject& VBO) const;

	GLuint create_shader_helper(GLint type, const std::string& shader_string);

};

class ProgramFactory {
public:
	static Program createWireframeShader(const std::string& fragment_data_name);
	static Program createFlatShader(const std::string& fragment_data_name);
	static Program createPhongShader(const std::string& fragment_data_name);
	static Program createShadowShader(const std::string& fragment_data_name);
	static Program createSkyboxShader(const std::string& fragment_data_name);
private:
	static std::string readShader(const std::string& shader);
};

#endif  // __HELPERS_H__
