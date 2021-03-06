//=============================================================================
// ■ control.hpp
//-----------------------------------------------------------------------------
//   接受输入并转换为视角变换的系统。
//=============================================================================

#ifndef INCLUDE_CONTROL_
#define INCLUDE_CONTROL_

#include <global.hpp>
#include "../GameObject/Camera/Camera.hpp"

namespace VM76 {
	//-------------------------------------------------------------------------
	// ● 所有控制方式的基类
	//-------------------------------------------------------------------------
	class Control : public Object {
	public:
		Camera* cam;

	public:
		virtual void init_control(Camera* cam) = 0;
		virtual void update_control() = 0;
	};
	//-------------------------------------------------------------------------
	// ● 演示模式
	//-------------------------------------------------------------------------
	class DemoView : public Control {
	public:
		void init_control(Camera* cam);
		void update_control();
	};
	//-------------------------------------------------------------------------
	// ● 上帝视角
	//-------------------------------------------------------------------------
	class GodView : public Control {
	public:
		struct {
			int quit;
			int forward, left, right, back;
			int zoomin, zoomout;
		} keys;

		void update_control();
		void init_control(Camera* cam);
	};
	//-------------------------------------------------------------------------
	// ● 第一人称视角
	//-------------------------------------------------------------------------
	class FirstPersonView : public Control {
	public:
		struct {
			int quit;
			int forward, left, right, back;
			int up, down;
		} keys;

		class Player {
		public:
			bool in_gui = false;
			float horizontal_angle = 0.0;
			float vertical_angle = 0.0;
			float speed = 0.0;
			glm::vec3 wpos = glm::vec3(0.0, 1.0, 0.0);
		} game_player;

	public:
		void init_control(Camera* cam);
		void update_control();
	};
}

#endif
