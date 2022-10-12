#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include "../../helper/HelperClass.h"
#include "../../view/ViewControl.h"
#include "../features/LightClass.h"
#include "../features/Skybox.h"

#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp>

namespace SceneEditor {

	class Skybox;

	enum ShaderMode {
		WIREFRAME = 0,
		FLAT = 1,
		PHONG = 2,
		SHADOW = 3,
		SKYBOX = 4,
		N_SHADER = 5
	};

	class Object {
	public:
		enum DisplayMode {
			MODE1 = 0,  // WIRE
			MODE2 = 1,  // (FLAT + WIRE) + PHONG
			MODE3 = 2,  // PHONG + PHONG
			MODE4 = 3,  // PHONG + MIRROR
			MODE5 = 4,  // PHONG + REFRACTION
			MODE6 = 5,  // FLAT + MIRROR
			MODE7 = 6,  // FLAT + REFRACTION
			MODE8 = 7,  // PHONG + MIRROR(DYNAMIC)
		};
		Object();
		void free();
		void draw(std::vector<Program>& programs, Light& light, ViewControl& view_control, Texture& depth_texture, Texture& skybox_texture, bool isEnvMap = false);
		void drawEnvMapping(std::vector<Program>& programs, Light& light, ViewControl& view_control, Texture& depth_texture, Texture& skybox_texture, glm::mat4& VPMatrix);
		void drawShadowMapping(Program& program);
		void loadFromOffFile(const std::string& path);
		void unitize();
		void update();
		void configEnvMap();
		void setDisplayMode(DisplayMode mode);
		DisplayMode getDisplayMode();

		void translate(float x, float y, float z);
		void rotate(float x, float y, float z);
		void scale(float change);
		void color(glm::vec3& color);
		void inverseColor();

		std::pair<bool, float> intersectRay(const glm::vec3& e, const glm::vec3& d, float near, float far) const;

		glm::mat4 getModelMatrix() const;
		glm::mat3 getNormalMatrix() const;
		std::vector<glm::mat4> getEnvVPMatrices() const;
	private:
		static std::pair<bool, float> intersectTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
			const glm::vec3& e, const glm::vec3& d, float near, float far);
		void drawWireframe(Program& program, Light& light, ViewControl& view_control, bool isEnvMap);
		void setPhongShading(Program& program, ViewControl& view_control, bool isEnvMap);
		void setFlatShading(Program& program, ViewControl& view_control, bool isEnvMap);
		void setPhongLighting(Program& program, Light& light, ViewControl& view_control, Texture& depth_texture);
		void setMirrorLighting(Program& program, Light& light, ViewControl& view_control, Texture& depth_texture, Texture& skybox_texture);
		void setRefractLighting(Program& program, Light& light, ViewControl& view_control, Texture& depth_texture, Texture& skybox_texture);
		void simpleDraw();
	private:
		std::vector<glm::vec3> m_vertices;
		std::vector<glm::vec3> m_vertex_normals;
		std::vector<int> m_indices;
		std::vector<float> m_model;  // 0,1,2 - translate, 3,4,5 - rotate, 6 - sacle
		glm::vec3 m_color;  // 0,1,2 - rgb
		DisplayMode m_mode;

		VertexBufferObject m_vbo;
		ElementBufferObject m_ebo;
		VertexBufferObject m_nbo;   // vbo for vertex normal

	public:
		FrameBufferObject env_fbo;
		Texture env_texture;
		static const int s_env_width = 2000;
		static const int s_env_height = 2000;
	private:
		glm::mat4 m_envVPMatrix;
	};

	class Geometry {
	public:
		Geometry();
		void init();
		void free();
		void bind();
		void configShadowMap();
		size_t size() const;
		void draw(std::vector<Program>& programs, ViewControl& view_control, Skybox& skybox);
		void addObjFromOffFile(const std::string& path);

		void addBunny();
		void addBumpyCube();
		void addCube();
		void addPlane();
		void deleteObject(int index);

		int intersectRay(const glm::vec3& e, const glm::vec3& d, float near, float far) const;

		const Object& operator[](size_t index) const;
		Object& operator[](size_t index);
		Light& getLight() { return m_light; }
		void redShadow();
	private:
		void getShadowTexture(Program& program, ViewControl& view_control);
		void getEnvTexture(std::vector<Program>& programs, ViewControl& view_control, Skybox& skybox);
	private:
		std::vector<Object> m_objs;
		VertexArrayObject m_vao;
		Light m_light;
		FrameBufferObject m_depth_fbo;
		Texture m_depth_texture;
	};
}
#endif  // __GEOMETRY_H__
