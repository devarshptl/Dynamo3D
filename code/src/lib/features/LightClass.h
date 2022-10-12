#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "TrackballClass.h"

namespace SceneEditor {

	class Light {
	public:
		Light(float x, float y, float z) {
			m_trackball.fromPosition({ x, y, z });
		}

		glm::vec3 getPosition() const {
			return m_trackball.toPosition();
		}

		void left(float length) {
			m_trackball.left(length);
		}
		void right(float length) {
			m_trackball.right(length);
		}
		void up(float length) {
			m_trackball.up(length);
		}
		void down(float length) {
			m_trackball.down(length);
		}
		void forward(float length) {
			m_trackball.forward(length);
		}
		void backward(float length) {
			m_trackball.backward(length);
		}

	private:
		Trackball m_trackball;
	};
}
#endif // __LIGHT_H__
