#pragma once
#include"math3d.h"
#include"prequest.h"

namespace NBengine
{
	class Shadow
	{
	protected:
		Texture *shadowMap;
		float mShadowFaceSize;
		matrix4f proj;
	public:
		Shadow(float shadowFacesize = 3.0);

		inline Texture*& getShadowMap() { return shadowMap; }
		inline matrix4f& getProj() { return proj; }
	};


	enum LightType {
		LT_point,
		LT_direction,
		LT_spot
	};


	class Light
	{
	protected:
		Shadow *shadowCaster;
	public:
		virtual ~Light() {
			DELETE(shadowCaster);
		}
		Light() { shadowCaster = 0; }
		virtual LightType getType() const = 0;
		virtual matrix4f calShadowTransform() const { return matrix4f().initIdentity(); }
		virtual matrix4f calShadowTransform(vector3f cameraPos) const { return matrix4f().initIdentity(); }
		inline virtual bool isCastShadow() const { return shadowCaster != NULL; }
		inline Shadow* getShadow() { return shadowCaster; }
	};


	class PointLight : public Light
	{
		vector3f pos, color;
		float Constant;
		float Linear;
		float Exponent;

	public:
		PointLight() {
			PointLight(vector3f(0, 0, 0), vector3f(1,1,1), 0, 0, 1.0f);
		}
		PointLight(const vector3f &pos, const vector3f & color, float constant, float linear, float exponent) : pos(pos),
			Constant(constant), color(color), Linear(linear), Exponent(exponent) {}

		inline LightType getType() const{
			static LightType type = LT_point;
			return type;
		}

		inline const vector3f & getPos() const{ return pos; }
		inline void setPos(const vector3f &p) { pos = p; }

		inline const vector3f & getColor() const{ return color; }
		inline void setColor(const vector3f &p) { color = p; }

		inline float getConstant() const { return Constant; }
		inline void setConstant(float c) { Constant = c; }

		inline float getLinear() const { return Linear; }
		inline void setLinear(float c) { Linear = c; }

		inline float getExponent() const { return Exponent; }
		inline void setExponent(float c) { Exponent = c; }
	};

	class DirectionLight : public Light
	{

		vector3f mPos;
		vector3f mDir;
		vector3f mColor;   // (1.0,1.0,1.0,1.0)
		float Intensity;
	public:
		DirectionLight() { DirectionLight(vector3f(0, 0, 0), vector3f(0, 0, 1), vector3f(1.0f, 1.0f, 1.0f), 1.0f); } 
		DirectionLight(const vector3f & mPos, const vector3f & mDir, const vector3f & mColor, float Intensity, bool isCastShadow = 1);
	

		inline LightType getType() const{
			static LightType type = LT_direction;
			return type;
		}

		inline const vector3f & getPos() const  { return mPos; }
		inline void setPos(const vector3f &p) { mPos = p; }

		inline const vector3f & getDir() const { return mDir; }
		inline void setDir(const vector3f &dir)  { mDir = dir; }

		inline const vector3f & getColor() const { return mColor; }
		inline void setColor(const vector3f &color) { mColor = color; }

		inline float getIntensity() const { return Intensity; }
		inline void setIntensity(float intnsity) { Intensity = intnsity; }

		virtual matrix4f calShadowTransform() const;

	};

	// 球面反射贴图（sphere map）
	// 计算球空间， 主相机的相对方向
	class SphereMirror
	{
	private:
		Texture *mEnvMap;
	public:
		SphereMirror();
		virtual ~SphereMirror();
		matrix4f getReflectTransform(const Quaternion& look) const;		// 和相机相对（xz轴相反）的坐标系
		Quaternion getMirrorRotation(const Quaternion& look) const;   // 相机方向绕局部Y转90度
		inline Texture* getEnvMap() const { return mEnvMap; }
	};

}