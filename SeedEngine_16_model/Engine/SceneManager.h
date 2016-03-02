#pragma once

#include"Node.h"
#include"prequest.h"

namespace Seed
{

	class SceneManager
	{
	protected:
		Node *root;
		RenderingEngine *renderingEngine;
		vector<GameObject*> mObjQue;



		Camera *mainCamera;

		PointLight *mDefaultPointLight;
		DirectionLight *mDefaultDirectionLight;				// default dir light

		PointLight *pointLight;
		DirectionLight *directionLight;

		Light *mActiveLight;


		Shader *normalMapShader;
		Shader *shadowShader;
		Shader *defaultShader;
		Shader *gaussBlurShader;
		Shader *transparentShader;
		Shader *sphereMap;
		Shader *fxaa;
		Shader *defered;
		Shader *skyShader;
		Shader *bloomShader;
		Texture* m_Rpostpocess;
		Texture* m_Rpostpocess1;

		Shader *mActiveShader;
		Texture* mR_ShadowFilter;
		SphereMirror *mSphereMirror;
		GameObject *mSky;
		IDirect3DCubeTexture9 *mEnvMap;



		enum {
			df_diffuse,
			df_normal,
			df_depth,
			df_N
		};
		Texture *mDeferedRT[df_N];
		

	public:
		SceneManager();
		virtual ~SceneManager();

		inline void setRenderingEngine(RenderingEngine *renderingEngine) { this->renderingEngine = renderingEngine; }
		virtual void renderOneFrame(vector<GameObject*> *q);
		virtual void _renderOneFrame(vector<GameObject*> *q);


		Node*& getRoot() { return root; }

		virtual void findVisableObjects(Node *t);
		virtual void update();
		virtual void renderScene();





		inline void setMainCamera(Camera * mainCamera) { this->mainCamera = mainCamera; }

		inline SceneManager* addPointLight(PointLight *p) {
			pointLight = p;
			return this;
		}

		inline PointLight* getPointLight() {
			if (!pointLight) return mDefaultPointLight;
			return pointLight;
		}
		inline SceneManager* addDirectionLight(DirectionLight *p) {
			directionLight = p;
			return this;
		}
		inline DirectionLight* getDirctionLight() {
			if (!directionLight) return mDefaultDirectionLight;
			return directionLight;
		}

		inline Light* getActiveLight()  { return mActiveLight; }
		void applyDeferedRendering(vector<GameObject*> *q);
		
		inline void addSky(GameObject *sky) { mSky = sky; }
		void applySky(GameObject *sky, Shader *shader, Camera *cam);
		void applyFXAA();

		void applyFilterDraw(int w, int h, Texture*r);
		void _applyGaussBlur(int w, int h, vector3f filterScale, Texture*s, Texture *t);
		void applyGaussBlur(int w, int h, Texture *img);
	};

}