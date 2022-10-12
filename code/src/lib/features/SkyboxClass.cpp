#include "Skybox.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"

#include <glm/gtc/type_ptr.hpp>

namespace SceneEditor {

	/* [SKYBOX FILES]
	* night_posx.png
	* night.negx.png
	* night_posy.png
	* night_negy.png
	* night_posz.png
	* night_negz.png
	*/
	std::vector<std::string> Skybox::skybox_faces = {
		"data/night_posx.png",
		"data/night_negx.png",
		"data/night_posy.png",
		"data/night_negy.png",
		"data/night_posz.png",
		"data/night_negz.png"
	};

	void Skybox::init() {
		m_vao.init();
		m_vbo.init();
		m_texture.init();
	}

	void Skybox::free() {
		m_vao.free();
		m_vbo.free();
		m_texture.free();
	}

	void Skybox::bind() {
		m_vao.bind();
	}

	void Skybox::update() {
		m_vbo.update(m_vertices);
	}

	void Skybox::configCubeMap() {
		m_texture.bind(GL_TEXTURE_CUBE_MAP);
		int width, height, nrChannels;
		for (unsigned int i = 0; i < skybox_faces.size(); i++)
		{
			unsigned char* data = stbi_load(skybox_faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap tex failed to load at path: " << skybox_faces[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void Skybox::draw(Program& program, ViewControl& view_control, bool isEnvMap) {
		program.bind();

		glm::mat4 VPMatrix, aspectRatioMatrix;
		if (isEnvMap) {
			VPMatrix = m_envVPMatrix;
			aspectRatioMatrix = glm::mat4(1.f);
		}
		else {
			VPMatrix = view_control.getProjMatrix() * glm::mat4(glm::mat3(view_control.getViewMatrix()));
			aspectRatioMatrix = view_control.getAspectRatioMatrix();
		}
		GLint uniVPMatrix = program.uniform("VPMatrix");
		glUniformMatrix4fv(uniVPMatrix, 1, GL_FALSE, glm::value_ptr(VPMatrix));
		GLint uniAR = program.uniform("AspectRatioMatrix");
		glUniformMatrix4fv(uniAR, 1, GL_FALSE, glm::value_ptr(aspectRatioMatrix));
		glActiveTexture(GL_TEXTURE0);
		m_texture.bind(GL_TEXTURE_CUBE_MAP);

		program.bindVertexAttribArray("position", m_vbo);
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
	}

	void Skybox::drawEnvMapping(Program& program, ViewControl& view_control, glm::mat4& envVPMatrix) {
		m_envVPMatrix = envVPMatrix;
		draw(program, view_control, true);
	}

	Texture Skybox::getTexture() const {
		return m_texture;
	}

	std::vector<glm::vec3> Skybox::m_vertices = {
		{-1.0f,  1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f, -1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},

		{-1.0f, -1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f},
		{-1.0f, -1.0f,  1.0f},

		{-1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f, -1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{ 1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f,  1.0f},
		{-1.0f,  1.0f, -1.0f},

		{-1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{ 1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f,  1.0f},
		{ 1.0f, -1.0f,  1.0f}
	};
}
