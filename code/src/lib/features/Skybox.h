#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "../../helper/HelperClass.h"
#include "../geometry/GeometryClass.h"
#include "../../view/ViewControl.h"

namespace SceneEditor {

	class Skybox {
	public:
		void init();
		void free();
		void bind();
		void update();
		void configCubeMap();
		void draw(Program& program, ViewControl& view_control, bool isEnvMap = false);
		void drawEnvMapping(Program& program, ViewControl& view_control, glm::mat4& envVPMatrix);
		Texture getTexture() const;
	private:
		VertexArrayObject m_vao;
		VertexBufferObject m_vbo;
		Texture m_texture;
		static std::vector<glm::vec3> m_vertices;
		static std::vector<std::string> skybox_faces;

		glm::mat4 m_envVPMatrix;
	};
}

#endif  // __SKYBOX_H__
