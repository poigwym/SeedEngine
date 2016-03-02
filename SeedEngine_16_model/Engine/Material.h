#pragma once
#include"prequest.h"
#include"Texture.h"
#include"Singleton.h"
#include"math3d.h"

namespace Seed
{
	class GameObject;
	class Camera;
	class VertexShader;
	class PixelShader;
	
	class MaterialManager : public Singleton<MaterialManager>
	{
	protected:
		map<string, PixelFormat> mStrToPF ;
		typedef map<string, Material*> MtlMap;

		MtlMap mMtlMap;
	public:
		MaterialManager();
		virtual ~MaterialManager();
		Material* createMaterial(const string& file);
	};


	class Material
	{
		friend class MaterialManager;
	protected:
		VertexShader *mVp;
		PixelShader *mFp;

		typedef map<string, Texture*> TextureMap;
		map<string, Texture*> mTextures;
		map<string, Texture*> mRTs;


		vector4f ambient;
		vector4f diffuse;
		vector4f specular;
		vector4f emessive;

		Texture *tex_diffuse;
		Texture *tex_normal;

		Texture *rt_shadow;

		bool mIsCastShadow;
		bool mIsGetShadow;

	public:
		Material();
		virtual ~Material();

		virtual void applyMaterial(GameObject *owner, Camera *cam, bool resetVP=true, bool resetFP=true);
		virtual void fireMaterial();

		Material* setDiffuseMap(Texture *tex_diffuse);
		Texture* getDiffuseMap();

		Material* setNormalMap(Texture *tex_normal);
		Texture* getNormalMap();

		Material* setVertexProgram(VertexShader *vp) { mVp = vp; }
		VertexShader *getVertexProgram() { return mVp; }

		Material* setPixelProgram(PixelShader *fp) { mFp = fp; }
		PixelShader* getPixelProgram() { return mFp; }

	};


}