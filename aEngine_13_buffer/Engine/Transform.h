
#pragma once

#include"prequest.h"
#include"math3d.h"

#include"Bug.h"

namespace NBengine
{

	enum TransformType {
		TS_parent = 0,
		TS_local = 1,
		TS_world
	};


	// Transform 层次结构
	// 用于计算local, final Transform矩阵
	// 更新pos, rot, scale, 要标记， 被层次树调用更新时再更新finalTransform
	// 子类重写update要加上updateTransform 

	class Transform
	{
	protected:
		Transform *parent;
		vector<Transform*> mChilds;

		vector3f mPos;
		Quaternion mRot;
		vector3f mScale;
		matrix4f mTransform;

		vector3f mFinalPos;
		Quaternion mFinalRot;
		vector3f mFinalScale;
		matrix4f mFinalTransform;

		bool mChildNeedUpdate;			// 子节点有更新，本节点只作为桥梁
		bool mSelfNeedUpdate;				// 自己的变量有变化
		bool mFaHasUpdate;					// 父节点变量有变化


	public:
		Transform() {
			mPos = vector3f(0, 0, 0);
			mRot = Quaternion(0, 0, 0, 1);
			mScale = vector3f(1, 1, 1);

			mFinalPos = mPos;
			mFinalRot = mRot;
			mFinalScale = mScale;

			mFaHasUpdate = 0;
			mChildNeedUpdate = 0;
			mSelfNeedUpdate = 0;

			parent = NULL;
		}

		virtual inline void setParent(Transform *t) {
			if (parent) {
				parent->detach(this);
			}
			parent = t;
		}
		virtual inline Transform* addChild(Transform *c) {
			mChilds.push_back(c);
			c->setParent(this);
			return this;
		}
		virtual inline void detach(Transform *c) {
			for(u32 i=0;i<mChilds.size();++i) 
			if (mChilds[i] == c) {
				mChilds.erase(mChilds.begin() + i);
				break;
			}
		}

		virtual inline Transform* setPos(const vector3f &pos) { 
			mPos = pos; 
			needUpdate();
			return this;
		}
		virtual inline Transform* setRot(const Quaternion &rot) {
			mRot = rot;
			needUpdate();
			return this;
		}
		virtual inline Transform* setScale(const vector3f &scale) {
			mScale = scale;
			needUpdate();
			return this;
		}


		// 局部坐标系transform			 
		inline matrix4f getTransform(TransformType type = TS_parent) const 
		{
			switch (type) {
			case TS_parent: return mFinalTransform;
			case TS_local: return mTransform;
			case TS_world: return mFinalTransform;
			}
		}


		inline const vector3f& getPos(TransformType type = TS_parent) const 
		{
			switch (type) {
			case TS_parent: return mFinalPos; 
			case TS_local: return mPos; 
			case TS_world: return mFinalPos;
			}
		}
		inline const Quaternion& getRot(TransformType type = TS_parent) const
		{
			switch (type) {
			case TS_parent: return mFinalRot;
			case TS_local: return mRot;
			case TS_world: return mFinalRot;
			}
		}
		inline const vector3f& getScale(TransformType type = TS_parent) const 
		{
			switch (type) {
			case TS_parent: return mFinalScale;
			case TS_local: return mScale;
			case TS_world: return mFinalScale;
			}
		}


		// 设为未更新
		inline void needUpdate() {
			mSelfNeedUpdate = true;
			callFaUpdate();
		}

		inline void callFaUpdate() {
			if (parent) {

				if (parent->mChildNeedUpdate ||
					parent->mSelfNeedUpdate) return;

				parent->mChildNeedUpdate = true;
				parent->callFaUpdate();
			}
		}

		inline void calTransform()
		{
			matrix4f translate, rotate, scale;
			translate.initTranslate(mPos);
			rotate.initRotateQuaternion(mRot);
			scale.initScale(mScale);
			mTransform = scale*rotate*translate;
		}

		inline void calFinalTransform()
		{
			matrix4f translate, rotate, scale;
			translate.initTranslate(mFinalPos);
			rotate.initRotateQuaternion(mFinalRot);
			scale.initScale(mFinalScale);
			mFinalTransform = scale*rotate*translate;
		}

		inline void updateTransform() {

			if (mFaHasUpdate || mSelfNeedUpdate) 
			{
				if (!parent) {
					mFinalPos = mPos;
					mFinalRot = mRot;
					mFinalScale = mScale;
				} else {
					mFinalPos = parent->getPos() + mPos;
					mFinalRot = (parent->getRot() * mRot);
					mFinalRot.normal();
					//mFinalScale = mScale + parent->getFinalScale();
					mFinalScale = mScale;
				}

				calTransform();
				calFinalTransform();

				for (u32 i = 0; i < mChilds.size(); ++i) {
					mChilds[i]->mFaHasUpdate = true;
					mChilds[i]->updateTransform();
				}

				mFaHasUpdate = 0;
				mSelfNeedUpdate = 0;
			}
			else if (mChildNeedUpdate)
			{
				for (u32 i = 0; i < mChilds.size(); ++i)
					mChilds[i]->updateTransform();

				mChildNeedUpdate = 0;
			}
		}

		inline matrix4f getFinalInvTransform() const {

			matrix4f translate, rotate, scale;
			translate.initTranslate(vector3f(-mFinalPos.getX(), -mFinalPos.getY(), -mFinalPos.getZ()));
			rotate.initRotateQuaternion(Quaternion(-mFinalRot.getX(), -mFinalRot.getY(), -mFinalRot.getZ(), mFinalRot.getW()));
			scale.initScale(vector3f(1.0f / mFinalScale.getX(), 1.0f / mFinalScale.getY(), 1.0f / mFinalScale.getZ()));
			return  translate*rotate*scale;
		}


		// 更新transform树 

		// update可留给子类扩展
		inline virtual void _update() { updateTransform(); }
		inline virtual void postUpdate() {
			for (int i = 0; i < mChilds.size(); ++i)
				mChilds[i]->update();
		}
		inline virtual void update() {
			_update();
			postUpdate();
		}


		virtual void _input() {}
		virtual void postInput() {
			for (int i = 0; i < mChilds.size(); ++i)
				mChilds[i]->input();
		}
		virtual void input() {
			_input();
			postInput();
		}

		
	};
}
