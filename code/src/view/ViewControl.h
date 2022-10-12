#ifndef __VIEW_CONTROL_H__
#define __VIEW_CONTROL_H__

#include "../lib/features/TrackballClass.h"

#include <utility>
#include <vector>

namespace SceneEditor {

	class ViewControl {
	public:

		ViewControl();

		float viewfar() { return m_f; }
		float viewnear() { return m_n; }
		float screenHeight() { return m_height; }
		float screenWidth() { return m_width; }

		glm::mat4 getViewMatrix();
		glm::mat4 getProjMatrix();
		glm::mat4 getAspectRatioMatrix();
		std::vector<glm::mat4> getShadowMatrices(const glm::vec3& lightPos);

		void setPerspective() { m_project_mode = PERSPECTIVE; }
		void setOrthographic() { m_project_mode = ORTHOGRAPHIC; }

		glm::vec2 worldCoordinateFromView(float x, float y);
		std::pair<glm::vec3, glm::vec3> getClickRay(float x, float y);


		void setTrackball() { m_camera_move_mode = TRACKBALL; }
		void setNormal() { m_camera_move_mode = NORMAL; }
		void left(float lengh);
		void right(float length);
		void up(float lengh);
		void down(float lengh);
		void forward(float length);
		void backward(float length);

		void setScreenSize(int height, int width);
		glm::vec3 getEyePosition() { return m_eyePos; }

	public:
		glm::mat4 getOrthoProjMatrix();
		glm::mat4 getPerspProjMatrix();

	private:
		enum ProjMode {
			ORTHOGRAPHIC,
			PERSPECTIVE
		};

		enum CameraMoveMode {
			NORMAL = 1,
			TRACKBALL = 2
		};
		float m_l;
		float m_r;
		float m_b;
		float m_t;
		float m_n;
		float m_f;
		float m_fov;

		float m_height;
		float m_width;

		glm::vec3 m_eyePos;
		glm::vec3 m_viewUp;

		Trackball m_trackball;

		ProjMode m_project_mode;
		CameraMoveMode m_camera_move_mode;
	};
}

#endif // __VIEW_CONTROL_H__