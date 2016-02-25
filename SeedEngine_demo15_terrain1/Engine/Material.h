#pragma once
#include"prequest.h"
#include"Texture.h"
#include"Singleton.h"

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


		bool mIsCastShadow;
		bool mIsGetShadow;

	public:
		Material();
		virtual ~Material();

		virtual void applyMaterial(GameObject *owner, Camera *cam, bool resetVP=true, bool resetFP=true);
		virtual void fireMaterial();
	};


}