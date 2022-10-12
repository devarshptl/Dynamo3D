#include "CallbackClass.h"

#include "HelperClass.h"

#include <glm/gtx/string_cast.hpp> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

namespace SceneEditor {

	BaseState::BaseState(Geometry& geometry, ViewControl& view_control)
		: m_geometry{ geometry }
		, view_control{ view_control } {}

	BaseState::~BaseState() {}

	void BaseState::windowSizeCallback(int width, int height) {
		glViewport(0, 0, width, height);
		view_control.setScreenSize(height, width);
	}

	void BaseState::mouseClickCallback(int button, int action,
		double screen_x, double screen_y) {}

	void BaseState::keyboardCallback(int key, int action) {}

	void BaseState::mouseMoveCallback(double screen_x, double screen_y) {}

	InsertState::InsertState(Geometry& geometry, ViewControl& view_control)
		: BaseState(geometry, view_control) {
	}

	InsertState::~InsertState() {
	}

	// [SCENE EDITOR::MESH]
	void InsertState::keyboardCallback(int key, int action) {
		if (GLFW_PRESS == action) {
			switch (key)
			{
			case  GLFW_KEY_1:
				printf("\n[SYSTEM INFO::INSERTION MODE] || [MODEL] CUBE\n");
				m_geometry.addCube();
				printf("[MODE INFO::CUBE] -> CUBE MODEL ADDED TO SCENE.\n");
				break;
			case GLFW_KEY_2:
				printf("\n[SYSTEM INFO::INSERTION MODE] || [MODEL] BUMPY CUBE\n");
				m_geometry.addBumpyCube();
				printf("[MODE INFO::BUMPY CUBE] -> BUMPY CUBE MODEL ADDED TO SCENE.\n");
				break;
			case  GLFW_KEY_3:
				printf("\n[SYSTEM INFO::INSERTION MODE] || [MODEL] BUNNY\n");
				m_geometry.addBunny();
				printf("[MODE INFO::BUNNY] -> BUNNY MODEL ADDED TO SCENE.\n");
				break;
			default:
				break;
			}
		}
	}

	MoveState::MoveState(Geometry& geometry, ViewControl& view_control)
		: BaseState(geometry, view_control) {}

	MoveState::~MoveState() {
		m_selected = -1;
	}

	void MoveState::mouseClickCallback(int button, int action,
		double screen_x, double screen_y) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (GLFW_PRESS == action) {
				auto p = view_control.getClickRay(screen_x, screen_y);
				glm::vec3 e = p.first;
				glm::vec3 d = p.second;
				m_selected = m_geometry.intersectRay(e, d,
					view_control.viewnear(), view_control.viewfar());
				if (m_selected != -1) {
					m_geometry[m_selected].inverseColor();
					std::cout << "Select: " << m_selected << std::endl;
				}
			}
			else if (GLFW_RELEASE == action) {
				m_geometry[m_selected].inverseColor();
			}
		}
	}

	void MoveState::mouseMoveCallback(double screen_x, double screen_y) {
		//
	}

	/* [SCENE EDITOR::MESH CONTROLLER]
	* Object Control
	* Render
	*/
	void MoveState::keyboardCallback(int key, int action) {
		if (GLFW_PRESS == action && m_selected != -1) {
			switch (key) {
			case  GLFW_KEY_W:
				m_geometry[m_selected].translate(0.f, 0.1f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] TRANSLATE\n");
				break;
			case  GLFW_KEY_S:
				m_geometry[m_selected].translate(0.f, -0.1f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] TRANSLATE\n");
				break;
			case  GLFW_KEY_A:
				m_geometry[m_selected].translate(-0.1f, 0.f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] TRANSLATE\n");
				break;
			case  GLFW_KEY_D:
				m_geometry[m_selected].translate(0.1f, 0.f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] TRANSLATE\n");
				break;
			case  GLFW_KEY_Q:
				m_geometry[m_selected].translate(0.f, 0.f, 0.1f);
				printf("[MODE INFO] MESH CONTROL || [MODE] TRANSLATE\n");
				break;
			case  GLFW_KEY_E:
				m_geometry[m_selected].translate(0.f, 0.f, -0.1f);
				printf("[MODE INFO] MESH CONTROL || [MODE] TRANSLATE\n");
				break;
			case  GLFW_KEY_F:
				m_geometry[m_selected].rotate(20.f, 0.f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] ROTATE\n");
				break;
			case  GLFW_KEY_G:
				m_geometry[m_selected].rotate(-20.f, 0.f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] ROTATE\n");
				break;
			case  GLFW_KEY_H:
				m_geometry[m_selected].rotate(0.f, 20.f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] ROTATE\n");
				break;
			case  GLFW_KEY_J:
				m_geometry[m_selected].rotate(0.f, -20.f, 0.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] ROTATE\n");
				break;
			case  GLFW_KEY_K:
				m_geometry[m_selected].rotate(0.f, 0.f, 20.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] ROTATE\n");
				break;
			case  GLFW_KEY_L:
				m_geometry[m_selected].rotate(0.f, 0.f, -20.f);
				printf("[MODE INFO] MESH CONTROL || [MODE] ROTATE\n");
				break;
			case GLFW_KEY_COMMA:
				m_geometry[m_selected].scale(.1f);
				printf("[MODE INFO] MESH CONTROL || [MODE] SCALE (+)\n");
				break;
			case GLFW_KEY_PERIOD:
				m_geometry[m_selected].scale(-.1f);
				printf("[MODE INFO] MESH CONTROL || [MODE] SCALE (-)\n");
				break;
			case GLFW_KEY_Z:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 1 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE1);
				printf("[MODE INFO] WIRE\n");
				break;
			case GLFW_KEY_X:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 2 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE2);
				printf("[MODE INFO] (FLAT + WIRE) + PHONG\n");
				break;
			case GLFW_KEY_C:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 3 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE3);
				printf("[MODE INFO] PHONG + PHONG\n");
				break;
			case GLFW_KEY_V:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 4 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE4);
				printf("[MODE INFO] PHONG + MIRROR\n");
				break;
			case GLFW_KEY_B:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 5 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE5);
				printf("[MODE INFO] PHONG + REFRACTION\n");
				break;
			case GLFW_KEY_N:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 6 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE6);
				printf("[MODE INFO] FLAT + MIRROR\n");
				break;
			case GLFW_KEY_M:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 7 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE7);
				printf("[MODE INFO] FLAT + REFRACTION\n");
				break;
			case GLFW_KEY_LEFT_SHIFT:
				printf("\n[SYSTEM INFO::DISPLAY MODE] MODE 8 || [STATUS] ACTIVE\n");
				m_geometry[m_selected].setDisplayMode(Object::MODE8);
				printf("[MODE INFO] PHONG + MIRROR(DYNAMIC)\n");
				break;
#define SET_OBJECT_COLOR(xx) \
                case GLFW_KEY_ ##xx :   \
                    m_geometry[m_selected].color(provided_color[xx - 1]); \
                    break;
				SET_OBJECT_COLOR(1)
					SET_OBJECT_COLOR(2)
					SET_OBJECT_COLOR(3)
					SET_OBJECT_COLOR(4)
					SET_OBJECT_COLOR(5)
					SET_OBJECT_COLOR(6)
					SET_OBJECT_COLOR(7)
					SET_OBJECT_COLOR(8)
					SET_OBJECT_COLOR(9)
#undef SET_POINT_COLOR

			default:
				break;
			}
		}
	}

	CameraState::CameraState(Geometry& geometry, ViewControl& view_control)
		: BaseState(geometry, view_control) {}

	CameraState::~CameraState() {}

	/* [CAMERA CONTROLLER]
	* Movement
	* Mode
	*/
	void CameraState::keyboardCallback(int key, int action) {
		if (GLFW_PRESS == action) {
			switch (key) {
			case GLFW_KEY_W:
				view_control.up(0.1);
				printf("[CAMERA MODE::MOVEMENT] UP\n");
				break;
			case GLFW_KEY_S:
				view_control.down(0.1);
				printf("[CAMERA MODE::MOVEMENT] DOWN\n");
				break;
			case GLFW_KEY_A:
				view_control.left(0.1);
				printf("[CAMERA MODE::MOVEMENT] LEFT\n");
				break;
			case GLFW_KEY_D:
				view_control.right(0.1);
				printf("[CAMERA MODE::MOVEMENT] RIGHT\n");
				break;
			case GLFW_KEY_Q:
				view_control.forward(0.1);
				printf("[CAMERA MODE::MOVEMENT] FORWARD\n");
				break;
			case GLFW_KEY_E:
				view_control.backward(0.1);
				printf("[CAMERA MODE::MOVEMENT] BACKWARD\n");
				break;
			case GLFW_KEY_Z:
				view_control.setTrackball();
				break;
			case GLFW_KEY_X:
				view_control.setNormal();
				break;
			case  GLFW_KEY_N:
				view_control.setPerspective();
				printf("[CAMERA MODE] PERSPECTIVE || [STATUS] ACTIVE\n");
				break;
			case  GLFW_KEY_M:
				view_control.setOrthographic();
				printf("[CAMERA MODE] ORTHOGRAPHIC || [STATUS] ACTIVE\n");
				break;
			}
		}
	}

	void CameraState::mouseClickCallback(int button, int action,
		double screen_x, double screen_y) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (GLFW_PRESS == action) {
			}
		}
	}

	DeleteState::DeleteState(Geometry& geometry,
		ViewControl& view_control)
		: BaseState(geometry, view_control) {}

	DeleteState::~DeleteState() {}

	/* [MOUSE CONTROLLER]
	* Selection
	*/
	void DeleteState::mouseClickCallback(int button, int action,
		double screen_x, double screen_y) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (GLFW_PRESS == action) {
				auto p = view_control.getClickRay(screen_x, screen_y);
				glm::vec3 e = p.first;
				glm::vec3 d = p.second;
				int selected = m_geometry.intersectRay(e, d,
					view_control.viewnear(), view_control.viewfar());
				if (selected != -1) {
					m_geometry.deleteObject(selected);
					std::cout << "[SYSTEM INFO::REMOVE MODE] -> DELETED ELEMENT INDEX:" << selected << std::endl;
				}
			}
		}
	}

	LightState::LightState(Geometry& geometry,
		ViewControl& view_control)
		: BaseState(geometry, view_control) {}

	LightState::~LightState() {}

	/* [LIGHT CONTROLLER]
	* Movement
	* Red Shadow
	*/
	void LightState::keyboardCallback(int key, int action) {
		if (GLFW_PRESS == action) {
			switch (key) {
			case GLFW_KEY_W:
				break;
			case GLFW_KEY_S:
				m_geometry.redShadow();
				break;
			case GLFW_KEY_A:
				m_geometry.getLight().left(0.1f);
				printf("[SYSTEM INFO::LIGHT MODE] -> [MOVEMENT] LEFT\n");
				break;
			case GLFW_KEY_D:
				m_geometry.getLight().right(0.1f);
				printf("[SYSTEM INFO::LIGHT MODE] -> [MOVEMENT] LEFT\n");
				break;
			}
		}
	}

	Callbacks::Callbacks(Geometry& geometry, ViewControl& view_control)
		: m_geometry{ geometry }
		, view_control{ view_control }
		, app_mode{ DEFAULT } {
		mouse_cursor = BaseState::ptr(new BaseState(m_geometry, view_control));
	}

	void Callbacks::toDefaultMode() {
		if (app_mode == DEFAULT) return;
		mouse_cursor.reset(new BaseState(m_geometry, view_control));
	}

	void Callbacks::toModeInsert() {
		if (app_mode == INSERT) return;
		mouse_cursor.reset(new InsertState(m_geometry, view_control));
	}

	void Callbacks::toModeMove() {
		if (app_mode == MOVE) return;
		mouse_cursor.reset(new MoveState(m_geometry, view_control));
	}

	void Callbacks::toModeCamera() {
		if (app_mode == CAMERA) return;
		mouse_cursor.reset(new CameraState(m_geometry, view_control));
	}

	void Callbacks::toModeDelete() {
		if (app_mode == REMOVE) return;
		mouse_cursor.reset(new DeleteState(m_geometry, view_control));
	}

	void Callbacks::toModeLight() {
		if (app_mode == LIGHT) return;
		mouse_cursor.reset(new LightState(m_geometry, view_control));
	}

	void Callbacks::mouseClickCallback(int button, int action, double screen_x, double screen_y) {
		mouse_cursor->mouseClickCallback(button, action, screen_x, screen_y);
	}

	/* [MODE CONTROLLER]
	* Insertion Mode
	* Move Mode
	* Remove Mode
	* Camera Mode
	* Light Mode
	*/
	void Callbacks::keyboardCallback(int key, int action) {
		if (GLFW_PRESS == action) {
			switch (key)
			{
			case  GLFW_KEY_I:
				toModeInsert();
				printf("\n[SYSTEM INFO::APP MODE] INSERTION MODE || [STATUS] ACTIVE\n");
				break;
			case GLFW_KEY_O:
				toModeMove();
				printf("\n[SYSTEM INFO::APP MODE] MOVE MODE || [STATUS] ACTIVE\n");
				break;
			case  GLFW_KEY_P:
				toModeDelete();
				printf("\n[SYSTEM INFO::APP MODE] REMOVE MODE || [STATUS] ACTIVE\n");
				break;
			case  GLFW_KEY_U:
				toModeCamera();
				printf("\n[SYSTEM INFO::APP MODE] CAMERA MODE || [STATUS] ACTIVE\n");
				break;
			case  GLFW_KEY_Y:
				toModeLight();
				printf("\n[SYSTEM INFO::APP MODE] LIGHT MODE || [STATUS] ACTIVE\n");
				break;
			default:
				break;
			}
		}
		mouse_cursor->keyboardCallback(key, action);
	}

	void Callbacks::windowSizeCallback(int width, int height) {
		mouse_cursor->windowSizeCallback(width, height);
	}

	void Callbacks::mouseMoveCallback(double screen_x, double screen_y) {
		mouse_cursor->mouseMoveCallback(screen_x, screen_y);
	}

	std::vector<glm::vec3> MoveState::provided_color = {
		{0.509f, 0.223f, 0.207f},
		{0.537f, 0.745f, 0.698f},
		{0.788f, 0.729f, 0.513f},
		{0.870f, 0.827f, 0.549f},
		{0.870f, 0.611f, 0.325f},
		{0.701f, 0.839f, 0.431f},
		{0.125f, 0.141f, 0.180f},
		{0.360f, 0.654f, 0.729f},
		{0.466f, 0.203f, 0.376f}
	};
}
