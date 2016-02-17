
#pragma once
#include"inputsystem.h"
#include"Transform.h"

namespace NBengine
{

	// camera 
	// Transform 结构， Quaternion 表示方向
	// 这里旋转只是绕局部坐标系轴旋转
	// 绑定到节点时, 作为子物体被更新finalTransform
	// 没有绑定到节点， 作为游离相机会自动调用更新finalTransform


	class Camera : public Transform
	{
	protected:
		static vector3f yAxis;

		float fov, zNear, zFar;
		int width, height;
	public:
		Camera() {
			Camera(vector3f(0, 0, 0), Quaternion(vector3f(0,1,0), pi),
				pi / 4, 800, 600, 1.0f, 3000.0f);
		}

		Camera(const vector3f & pos, const Quaternion &rot,
			float fov, int w, int h, float zNear, float zFar) : fov(fov), width(w), height(h), zNear(zNear), zFar(zFar) {
			setPos(pos);
			setRot(rot);

			if(parent == NULL)				// 没有绑定到节点，要手动更新finalTransform
				updateTransform();
		}

		virtual ~Camera() {}

		void move(const vector3f &dir, float amt) { 
			setPos(getPos(TS_local) + dir*amt);
			if (parent == NULL)				// 没有绑定到节点，要手动更新finalTransform
				updateTransform();
		}
		inline  const vector3f& getEye() const { return getPos(); }

		inline void rotateY(float rad, const vector3f &axis = yAxis) {
			setRot(getRot(TS_local) * Quaternion(axis, rad));
			if (parent == NULL)				// 没有绑定到节点，要手动更新finalTransform
				updateTransform();
		}
		
		inline void rotateX(float rad) {
			vector3f Haxis = yAxis.cross(getForward(TS_local)).normal();
			setRot(getRot(TS_local) * Quaternion(Haxis, rad));
			if (parent == NULL)				// 没有绑定到节点，要手动更新finalTransform
				updateTransform();
		}

		matrix4f getView() const {
			matrix4f view;
			return view.initView(getPos(TS_parent), getForward(), getUp());
		}

		matrix4f getProjection() const {
			matrix4f proj;
			return proj.initProjection(fov, (float)width / height, zNear, zFar);
		}

		inline vector3f getForward(TransformType type = TS_parent) const { return getRot(type).getForward(); }
		inline vector3f getUp(TransformType type = TS_parent) const { return getRot(type).getUp(); }
		inline vector3f getRight(TransformType type = TS_parent) const { return getRot(type).getRight(); }
		inline vector3f getLeft(TransformType type = TS_parent) const { return -getRot(type).getRight(); }

		void input();

		inline void update()
		{
			input();
			if (parent == NULL)				// 没有绑定到节点，要手动更新finalTransform
				updateTransform();
		}



	};

}