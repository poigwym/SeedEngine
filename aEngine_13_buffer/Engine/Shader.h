


#pragma once

#include"prequest.h"
#include"lighting.h"

namespace NBengine
{

	class ShaderData
	{
	public:
		ShaderData() {}
		virtual ~ShaderData() {}

		inline void addData(const string &name, float f) { mFMap[name] = f; }
		inline void addData(const string &name, const vector3f& v) { mVec3Map[name] = v; }
		inline void addData(const string &name, const matrix4f& m) { mMat4Map[name] = m; }

		inline map<string, float>& getFMap() { return mFMap; }
		inline map<string, vector3f>& getVec3Map() { return mVec3Map; }
		inline map<string, matrix4f>& getMatMap() { return mMat4Map; }

	private:
	protected:
		map<string, float> mFMap;
		map<string, vector3f> mVec3Map;
		map<string, matrix4f> mMat4Map;
		map<string, Light*> mLMap;

	};






	class Shader
	{
	private:
		ID3DXEffect *es;
		ShaderData *mData;

	public:
		Shader()  {
			es = 0;
			mData = new ShaderData;
		}
		Shader(const string &file);

		virtual ~Shader() {
			DELETE(mData);
			RELEASE(es);
		}

		ID3DXEffect* getInstance() { return es; }

		inline virtual int begin(const string & name) {
			es->SetTechnique(name.c_str());
			UINT c;
			es->Begin(&c, 0);
			//es->BeginPass(0);
			return c;
		}

		inline virtual void beginPass(int i) {
			es->BeginPass(i);
		}

		inline virtual void endPass() {
			es->EndPass();
		}
		
		inline virtual void end() {
			es->End();
		}

		inline void CommitChange() { es->CommitChanges(); }

		inline void addData(const string &name, float f){ mData->addData(name, f); }
		inline void addData(const string &name, const vector3f& v) { mData->addData(name, v); }
		inline void addData(const string &name, const matrix4f &m) { mData->addData(name, m); }

		virtual void update(GameObject *obj, Camera *cam, SceneManager *renderingEngine);



		inline virtual void setValue(const string &name, float v) {
			es->SetFloat(name.c_str(), v);
		}


		inline virtual void setValue(const string &name, const vector3f &v) {
			es->SetVector(name.c_str(), &D3DXVECTOR4(v.getX(), v.getY(), v.getZ(), 0));
		}

		inline virtual void setValue(const string &name, const matrix4f &mat) {
			es->SetMatrix(name.c_str(), (D3DXMATRIX*)&mat);
		}

		inline virtual void setValue(const string& name, LPDIRECT3DTEXTURE9 tex) {
			es->SetTexture(name.c_str(), tex);
		}

		 virtual void setValue(const string& name, Texture* tex);

		inline virtual void setValue(const string& name, Light* light)
		{
			//LT_point,
				//LT_direction,
				//LT_spot
			switch (light->getType())
			{
			case LT_point: {
											 PointLight *l = dynamic_cast<PointLight*>(light);
											 setValue(name + ".pos", l->getPos());
											 setValue(name + ".color", l->getColor());
											 setValue(name + ".Constant", l->getConstant());
											 setValue(name + ".Linear", l->getLinear());
											 setValue(name + ".Exponent", l->getExponent());
											 break;
			}
			case LT_direction:{
													DirectionLight *l = dynamic_cast<DirectionLight*>(light);

													setValue(name + ".pos", l->getPos());
													setValue(name + ".dir", l->getDir());
													setValue(name + ".color", l->getColor());
													setValue(name + ".Intensity", l->getIntensity());
													break;
			}
			case LT_spot: {
											break;
			}
			}

		}

	};

}