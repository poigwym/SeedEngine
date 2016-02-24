#include"lighting.h"
#include"RenderingEngine.h"
#include"Material.h"

namespace Seed
{

	Shadow::Shadow(float shadowFacesize ) :mShadowFaceSize(shadowFacesize) 
	{
		proj.initProjection(pi / 4, 1.0, 1.0, 3000);
		mtl = MaterialManager::getInstance().createMaterial("shadow.material");
	}

	Shadow ::~Shadow() {
	}



	DirectionLight::DirectionLight(const vector3f & mPos, const vector3f & mDir, const vector3f & mColor, float Intensity, bool isCastShadow) : mPos(mPos), mDir(mDir), mColor(mColor), Intensity(Intensity)
	{
		if (isCastShadow)
			shadowCaster = new Shadow;
	}

	matrix4f DirectionLight::calShadowTransform() const
	{
		vector3f right = vector3f(0, 1, 0).cross(mDir).normal();
		vector3f up = mDir.cross(right).normal();
		return matrix4f().initView(mPos, mDir, up) * shadowCaster->getProj();
	}



	SphereMirror::SphereMirror()
	{
		mEnvMap = RenderingEngine::getInstancePtr()->createRenderTarget(
			512, 512, PF_A8R8G8B8);
	}

	SphereMirror::~SphereMirror()
	{
	}



	matrix4f SphereMirror::getReflectTransform(const Quaternion&look) const
	{
		return matrix4f().initView(vector3f(0, 0, 0), -look.getForward(), look.getUp());
	}

	Quaternion SphereMirror::getMirrorRotation(const Quaternion& look) const
	{
		return look * Quaternion(look.getUp(), pi);
	}

}