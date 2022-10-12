#include "ViewControl.h"

#include "../lib/features/MacroClass.h"

#include <glm/gtx/string_cast.hpp> 

#include <iostream>

namespace SceneEditor {

	ViewControl::ViewControl()
		: m_l{ -1.f }, m_r{ 1.f }
		, m_b{ -1.f }, m_t{ 1.f }
		, m_n{ 0.1f }, m_f{ 20.f }
		, m_fov{ 45.f }
		, m_viewUp{ 0.f, 1.f, 0.f }
		, m_trackball{ 5.f, 90.f, 0.f }
		, m_project_mode{ PERSPECTIVE }
		, m_camera_move_mode{ TRACKBALL } {
		m_eyePos = m_trackball.toPosition();
	}

	glm::mat4 ViewControl::getViewMatrix() {
		glm::mat4 view = glm::lookAt(m_eyePos, glm::vec3(0.f), m_viewUp);

		return view;
	}

	glm::mat4 ViewControl::getOrthoProjMatrix() {
		return glm::ortho(m_l, m_r, m_l, m_r, m_n, m_f);
	}

	glm::mat4 ViewControl::getProjMatrix() {
		if (m_project_mode == PERSPECTIVE) {
			return getPerspProjMatrix();
		}
		else if (m_project_mode == ORTHOGRAPHIC) {
			return getOrthoProjMatrix();
		}
		else {
			ASSERT(false, "Invalid projection mode");
		}
	}

	glm::mat4 ViewControl::getPerspProjMatrix() {
		return glm::perspective(glm::radians(m_fov), 1.f, m_n, m_f);
	}

	glm::mat4 ViewControl::getAspectRatioMatrix() {
		return glm::scale(glm::mat4(1.f), glm::vec3(static_cast<float>(m_height) / static_cast<float>(m_width), 1.f, 1.f));
	}

	std::vector<glm::mat4> ViewControl::getShadowMatrices(const glm::vec3& lightPos) {
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)1024 / (float)1024, m_n, m_f);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		return shadowTransforms;
	}

	glm::vec2 ViewControl::worldCoordinateFromView(float x, float y) {
		glm::vec4 world = glm::inverse(getViewMatrix()) * glm::inverse(getOrthoProjMatrix()) * glm::inverse(getViewMatrix()) * glm::vec4(x, y, 0.f, 1.f);
		return { world[0], world[1] };
	}

	std::pair<glm::vec3, glm::vec3> ViewControl::getClickRay(float x, float y) {
		glm::mat4 convert = glm::inverse(getAspectRatioMatrix() * getProjMatrix() * getViewMatrix());

		glm::vec4 start = convert * glm::vec4(x, y, -1.f, 1.f);
		glm::vec4 end = convert * glm::vec4(x, y, 1.f, 1.f);
		start /= start[3];
		end /= end[3];
		// std::cout << "start=" << glm::to_string(start) << ", end=" << glm::to_string(end) << std::endl;
		return { glm::vec3(start), glm::normalize(glm::vec3(end - start)) };
	}


	void ViewControl::left(float length) {
		if (m_camera_move_mode == TRACKBALL) {
			m_trackball.left(length);
			m_eyePos = m_trackball.toPosition();
		}
		else {
			m_eyePos[0] -= length;
			m_trackball.fromPosition(m_eyePos);
		}
	}

	void ViewControl::right(float length) {
		if (m_camera_move_mode == TRACKBALL) {
			m_trackball.right(length);
			m_eyePos = m_trackball.toPosition();
		}
		else {
			m_eyePos[0] += length;
			m_trackball.fromPosition(m_eyePos);
		}
	}

	void ViewControl::up(float length) {
		if (m_camera_move_mode == TRACKBALL) {
			m_trackball.up(length);
			m_eyePos = m_trackball.toPosition();
		}
		else {
			m_eyePos[1] += length;
			m_trackball.fromPosition(m_eyePos);
		}
	}

	void ViewControl::down(float length) {
		if (m_camera_move_mode == TRACKBALL) {
			m_trackball.down(length);
			m_eyePos = m_trackball.toPosition();
		}
		else {
			m_eyePos[1] -= length;
			m_trackball.fromPosition(m_eyePos);
		}
	}

	void ViewControl::forward(float length) {
		if (m_camera_move_mode == TRACKBALL) {
			m_trackball.forward(length);
			m_eyePos = m_trackball.toPosition();
		}
		else {
			m_eyePos[2] -= length;
			m_trackball.fromPosition(m_eyePos);
		}
	}

	void ViewControl::backward(float length) {
		if (m_camera_move_mode == TRACKBALL) {
			m_trackball.backward(length);
			m_eyePos = m_trackball.toPosition();
		}
		else {
			m_eyePos[2] += length;
			m_trackball.fromPosition(m_eyePos);
		}
	}

	void ViewControl::setScreenSize(int height, int width) {
		m_height = height;
		m_width = width;
	}
};
