#ifndef __CALLBACKS_H__
#define __CALLBACKS_H__

#include "../lib/geometry/GeometryClass.h"

#include <memory>

namespace SceneEditor {

	class BaseState {
	public:
		typedef std::unique_ptr<BaseState> ptr;
		BaseState(Geometry& geometry, ViewControl& view_control);
		virtual ~BaseState();
		virtual void windowSizeCallback(int width, int height);
		virtual void mouseClickCallback(int button, int action,
			double xworld, double yworld);
		virtual void keyboardCallback(int key, int action);
		virtual void mouseMoveCallback(double xworld, double yworld);
	protected:
		Geometry& m_geometry;
		ViewControl& view_control;
	};

	class InsertState : public BaseState {
	public:
		InsertState(Geometry& geometry, ViewControl& view_control);
		virtual ~InsertState();
		void keyboardCallback(int key, int action);
	};

	class MoveState : public BaseState {
	public:
		MoveState(Geometry& geometry, ViewControl& view_control);
		virtual ~MoveState();
		void mouseClickCallback(int button, int action,
			double xworld, double yworld) override;
		void mouseMoveCallback(double xworld, double yworld) override;
		void keyboardCallback(int key, int action) override;
	private:
		int m_selected = -1;
		static std::vector<glm::vec3> provided_color;
	};

	class CameraState : public BaseState {
	public:
		CameraState(Geometry& geometry, ViewControl& view_control);
		virtual ~CameraState();
		void mouseClickCallback(int button, int action,
			double xworld, double yworld) override;
		void keyboardCallback(int key, int action) override;
	private:
	};

	class DeleteState : public BaseState {
	public:
		DeleteState(Geometry& geometry, ViewControl& view_control);
		virtual ~DeleteState();
		void mouseClickCallback(int button, int action,
			double xworld, double yworld) override;
	};

	class LightState : public BaseState {
	public:
		LightState(Geometry& geometry, ViewControl& view_control);
		virtual ~LightState();
		void keyboardCallback(int key, int action) override;
	};

	class Callbacks {
	public:
		Callbacks(Geometry& geometry, ViewControl& view_control);

		void toDefaultMode();
		void toModeInsert();
		void toModeMove();
		void toModeCamera();
		void toModeDelete();
		void toModeLight();
		void mouseClickCallback(int button, int action, double xworld, double yworld);
		void keyboardCallback(int key, int action);
		void windowSizeCallback(int width, int height);
		void mouseMoveCallback(double xworld, double yworld);
	private:
		enum AppMode {
			DEFAULT = 0,
			INSERT = 1,
			MOVE = 2,
			CAMERA = 3,
			REMOVE = 4,
			LIGHT = 5
		};
		BaseState::ptr mouse_cursor;
		Geometry& m_geometry;
		ViewControl& view_control;
		AppMode app_mode;
	};
}
#endif // __CALLBACKS_H__
