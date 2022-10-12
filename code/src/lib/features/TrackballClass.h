#ifndef __TRACKBALL_H__
#define __TRACKBALL_H__

#include <glm/glm.hpp> // glm::vec3
#include <glm/vec3.hpp> // glm::vec3
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace SceneEditor {

	class Trackball {
	public:
		Trackball();
		Trackball(float radius, float beta, float phi);
		void left(float length);
		void right(float length);
		void up(float length);
		void down(float length);
		void forward(float length);
		void backward(float length);
		glm::vec3 toPosition() const;
		void fromPosition(const glm::vec3& position);

	private:
		static float d2r(float d);
		static float r2d(float d);
		float lengthToDegree(float length);
	private:
		float m_beta;
		float m_phi;
		float m_radius;
	};
}

#endif  // __TRACKBALL_H__
