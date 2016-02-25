#include"Material.h"
#include"shaderprogram.h"
#include"d3d9RenderingEngine.h"
#include"TextureManager.h"
#include"RenderingEngine.h"
#include"GameObject.h"
#include"camera.h"
#include"lighting.h"

namespace Seed
{
	MaterialManager* Singleton<MaterialManager>::instance = 0;

	MaterialManager::MaterialManager()
	{
		instance = this;


		map<string, PixelFormat> &m = mStrToPF;
		m["PF_A8R8G8B8"] = PF_A8R8G8B8;
		m["PF_X8R8G8B8"] = PF_X8R8G8B8;
		m["PF_R8G8B8"] = PF_R8G8B8;
		m["PF_R5G6B5"] = PF_R5G6B5;
		m["PF_A1R5G5B5"] = PF_A1R5G5B5;
		m["PF_X1R5G5B5"] = PF_X1R5G5B5;
		m["PF_A4R4G4B4"] = PF_A4R4G4B4;
		m["PF_R32"] = PF_R32;
	}

	MaterialManager::~MaterialManager() {
		for (MtlMap::iterator i = mMtlMap.begin(); i != mMtlMap.end(); ++i)
			DELETE(i->second);
	}

	Material* MaterialManager::createMaterial(const string& file)
	{
		Material* mtl = new Material;

		ifstream in(file.c_str(), ios::in);
		int c = 0;
		string buf;
		in >> buf;
		assert(buf == "material");
		in >> buf;
		Log << "material name -- " << buf << endl;

		// insert to map
		if (mMtlMap[buf])
			return mMtlMap[buf];
		mMtlMap[buf] = mtl;

		while (in >> buf)
		{
			if (buf[0] == '{') ++c;
			else if (buf[0] == '}') --c;

			if (buf == "pro_vertex")
			{
				string f, entry, version;
				in >> f >> entry >> version;
				Log << "pro_vertex" << ' ' << f << ' ' << entry << ' ' << version << endl;
				mtl->mVp = new VertexShader(d3d9RenderingEngine::getDevice(), f, entry, version);
			}
			else if (buf == "pro_pixel")
			{
				string f, entry, version;
				in >> f >> entry >> version;
				Log << "pro_pixel" << ' ' << f << ' ' << entry << ' ' << version << endl;
				mtl->mFp = new PixelShader(d3d9RenderingEngine::getDevice(), f, entry, version);
			}

			else if (buf == "texture")
			{
				Log << "texture " << '\n' << '{' << endl;
				in >> buf;		// '{'
				string name, path, fmt;
				int w, h, ifmt, mip;
				in >> name >> path >> w >> h >> fmt >> mip;
				if (mStrToPF.find(fmt) == mStrToPF.end())
					ifmt = PF_UNKNOWN;
				else
					ifmt = mStrToPF[fmt];
				Log << name << '-' << path << '-' << w << '-' << h << '-' << fmt << '-' << mip << endl;
				mtl->mTextures[name] = TextureManager::getInstancePtr()->
					create(w, h, (PixelFormat)ifmt, mip, path);
				in >> buf;		// '}'
				Log << buf << endl;

			}
			else if (buf.substr(0, 2) == "rt")
			{
				// render target
				string  fmt;
				int w, h, ifmt;
				in >> w >> h >> fmt;
				if (mStrToPF.find(fmt) == mStrToPF.end())
					ifmt = PF_UNKNOWN;
				else
					ifmt = mStrToPF[fmt];
				mtl->mRTs[buf] = RenderingEngine::getInstance().createRenderTarget(w, h, (PixelFormat)ifmt);
			}
		}

		in.close();

		return mtl;
	}


	Material::Material()
	{
		mVp = 0;
		mFp = 0;
		mIsCastShadow = true;
		mIsGetShadow = true;


	}

	Material::~Material()
	{
	}


	void Material::applyMaterial(GameObject *owner, Camera *cam, bool resetVP, bool resetFP)
	{
		Log << "begin update vp uniform" << endl;
		UniformMap &vpUniform = mVp->getUniformMap();
		for (UniformMap::iterator i = vpUniform.begin(); i != vpUniform.end(); ++i)
		{
			const string &s = i->first;
			D3DXHANDLE h = vpUniform[s];
			if (s == "MW" && owner)
				mVp->setValue(h, owner->getTransform());
			else if (s == "MV" && cam)
				mVp->setValue(h, cam->getView());
			else if (s == "MP" && cam)
				mVp->setValue(h, cam->getProjection());
			else if (s == "MVP" && cam)
				mVp->setValue(h, cam->getView()*cam->getProjection());
			else if (s == "MLVP" && owner)
			{
				Light *l = owner->getActiveLight();
				if (l) {
					mVp->setValue(h, l->calShadowTransform());
				}
			}
			else if (s == "camPos" && cam)
				mVp->setValue(h, cam->getPos());
		}
		Log << "begin update fp uniform" << endl;

		UniformMap &fpUniform = mFp->getUniformMap();
		for (UniformMap::iterator i = fpUniform.begin(); i != fpUniform.end(); ++i)
		{
			const string &s = i->first;
			D3DXHANDLE h = i->second;

			Log << s << endl;

			if (s == "MW" && owner)
				mFp->setValue(h, owner->getTransform());
			else if (s == "MV" && cam)
				mFp->setValue(h, cam->getView());
			else if (s == "MP" && cam)
				mFp->setValue(h, cam->getProjection());
			else if (s == "MVP" && cam)
				mFp->setValue(h, cam->getView()*cam->getProjection());

			else if (s == "camPos" && cam)
				mFp->setValue(h, cam->getPos());


			// set texture to sampler
			if (s.substr(0, 4) == "samp")
			{
				string tmp = string("tex_") + s.substr(5, s.length() - 5);
				Log << tmp << endl;
				if (mTextures[tmp])
					mFp->setValue(h, mTextures[tmp]);

				if (s == "samp_shadow") 
				{
					// tex_shadow是shadow材质里的RT0
					mFp->setValue(h, owner->getActiveLight()->getShadow()->getMaterial()->mRTs["rt0"]);
				}
			}
		}

		Log << "begin bind shader program " << endl;

		if (resetVP)
			mVp->bindVertexProgram();
		if (resetFP)
			mFp->bindPixelProgram();


		if (resetVP || resetFP)
		{
			for (TextureMap::iterator i = mRTs.begin(); i != mRTs.end(); ++i)
			{
				// set render target  rt id 
				i->second->bindAsRenderTarget(atoi(i->first.substr(2, i->first.length() - 2).c_str()));
			}
		}
	}


	void Material::fireMaterial()
	{
		mVp->fireProgram();
		mFp->fireProgram();
	}

}
