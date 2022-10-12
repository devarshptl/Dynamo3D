#include "TrackballClass.h"

#include <iostream>

namespace SceneEditor {

	Trackball::Trackball() : Trackball(0.f, 0.f, 0.f) {}

	Trackball::Trackball(float radius, float beta, float phi) {
		m_radius = radius;
		m_beta = beta;
		m_phi = phi;
	}

	void Trackball::left(float length) {
		m_phi = std::fmod(m_phi - lengthToDegree(length) + 360, 360.f);
	}

	void Trackball::right(float length) {
		m_phi = std::fmod(m_phi + lengthToDegree(length), 360.f);
	}

	void Trackball::up(float length) {
		float tmp = m_beta - lengthToDegree(length);
		if (tmp > 0) {
			m_beta = tmp;
		}
	}

	void Trackball::down(float length) {
		float tmp = m_beta + lengthToDegree(length);
		if (tmp < 180) {
			m_beta = tmp;
		}
	}

	void Trackball::forward(float length) {
		if (m_radius - length > 0) {
			m_radius -= length;
		}
	}

	void Trackball::backward(float length) { m_radius += length; }

	glm::vec3 Trackball::toPosition() const {
		float phi = d2r(m_phi);
		float beta = d2r(m_beta);
		float x = m_radius * std::sin(beta) * std::sin(phi);
		float y = m_radius * std::cos(beta);
		float z = m_radius * std::sin(beta) * std::cos(phi);
		// std::cout << "r=" << m_radius << ", beta=" << m_beta << ", phi=" << m_phi << std::endl;;
		return { x, y, z };
	}

	void Trackball::fromPosition(const glm::vec3& position) {
		m_radius = glm::length(position);
		float beta = std::acos(position[1] / m_radius);

		float sin_beta = std::sin(beta);
		float tmp_phi = std::asin(position[0] / (m_radius * sin(beta)));
		float cos_phi = position[2] / (m_radius * sin_beta);
		if (cos_phi < 0) {
			tmp_phi = ((float)M_PI) - tmp_phi;
		}
		else if (tmp_phi < 0) {
			tmp_phi += (2 * ((float)M_PI));
		}
		float phi = tmp_phi;

		m_phi = r2d(phi);
		m_beta = r2d(beta);
	}

	float Trackball::d2r(float d) { return (d / 180.f) * ((float)M_PI); }
	float Trackball::r2d(float r) { return (r * 180.f) / ((float)M_PI); }

	float Trackball::lengthToDegree(float length) {
		return std::fmod(360 * (length / (2 * ((float)M_PI) * m_radius)), 360);
	}
}
