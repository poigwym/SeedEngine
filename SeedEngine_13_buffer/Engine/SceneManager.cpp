#include"SceneManager.h"
#include"CoreHeaders.h"
#include"Shader.h"
#include"d3d9Headers.h"

namespace NBengine
{
	SceneManager::SceneManager() {
		root = new Node;

		renderingEngine = 0;

		mainCamera = 0;

		mDefaultPointLight = 0;
		mDefaultDirectionLight = 0;

		pointLight = 0;
		directionLight = 0;


		RenderingEngine *renderingEngine = RenderingEngine::getInstancePtr();

		mDefaultPointLight = new PointLight(vector3f(0, 1, 0), vector3f(1, 1, 1), 0, 0, 1);
		mDefaultDirectionLight = new DirectionLight(vector3f(0, 10, 10), vector3f(0, -1, -1), vector3f(1, 1, 1), 1, true);


		mActiveShader = 0;

		defaultShader = new Shader("defaultShader.hlsl");

		normalMapShader = new Shader("normalMapping.hlsl");
		shadowShader = new Shader("shadowMap.hlsl");
		gaussBlurShader = new Shader("filter-gaussBlur.hlsl");


		transparentShader = new Shader("Transparent.hlsl");

		sphereMap = new Shader("SphereMap.hlsl");
		fxaa = new Shader("filter-fxaa.hlsl");

		defered = new Shader("defered.hlsl");

		skyShader = new Shader("Sky.hlsl");



		if (FAILED(D3DXCreateCubeTextureFromFile(
			d3d9RenderingEngine::getDevice(), "picture/skybox/SkyBox.dds", &mEnvMap)))
			throw("envmap error");


		mActiveLight = mDefaultDirectionLight;

		mR_ShadowFilter = renderingEngine->createRenderTarget
			(512, 512, PXFMT_R32);

		m_Rpostpocess = renderingEngine->createRenderTarget
			(800, 600, PXFMT_A8R8G8B8);
		m_Rpostpocess1 = renderingEngine->createRenderTarget
			(800, 600, PXFMT_A8R8G8B8);

		mSphereMirror = new SphereMirror;
		
		mBill = new Billboard;
		bloomShader = new Shader("BillboardBloom.hlsl");


		for (u32 i = 0; i < df_N; ++i)
		{
			if (i == df_depth)
				mDeferedRT[i] = renderingEngine->createRenderTarget
				(renderingEngine->getWidth(), renderingEngine->getHeight(), PXFMT_R32);
			else
				mDeferedRT[i] = renderingEngine->createRenderTarget
				(renderingEngine->getWidth(), renderingEngine->getHeight(), PXFMT_A8R8G8B8);
		}
	}

	SceneManager:: ~SceneManager()
	{
		DELETE(normalMapShader);
		DELETE(defaultShader);
		DELETE(mDefaultDirectionLight);
		DELETE(mDefaultPointLight);
		DELETE(shadowShader);
		DELETE(transparentShader);
		DELETE(sphereMap);
		DELETE(fxaa);
		DELETE(defered);
		DELETE(mSphereMirror);
		DELETE(skyShader);

		DELETE(mBill);
		DELETE(bloomShader);
		
	}


	void SceneManager::update()
	{
		root->update();

		mObjQue.clear();

		if (mainCamera)
			mainCamera->update();
		assert(mainCamera != 0);
		renderingEngine->setView(mainCamera->getView());
		renderingEngine->setProj(mainCamera->getProjection());

		findVisableObjects(root);
	}

	void SceneManager::findVisableObjects(Node *t)
	{
		t->processQueue(&mObjQue);
	}



	void SceneManager::renderScene()
	{
		assert(renderingEngine != 0);

		RenderingEngine::getInstancePtr()->beginFrame();

		this->renderOneFrame(&mObjQue);

		RenderingEngine::getInstancePtr()->endFrame();
	}

	void SceneManager::renderOneFrame(vector<GameObject*> *q)
	{

		// sky sphere
		applySky(mSky, skyShader, mainCamera);

		// billboard
		mBill->update(mainCamera);
		mBill->setPos(vector3f(5, 10, 0))->updateTransform();

		bloomShader->begin("BillboardBloom");
		bloomShader->setValue("MW", mBill->getTransform());
		//defaultShader->setValue("t_texture", obj->getTexture());
		bloomShader->setValue("camPos", mainCamera->getPos());
		bloomShader->setValue("MV", mainCamera->getView());
		bloomShader->setValue("MP", mainCamera->getProjection());
		bloomShader->setValue("MVP", mainCamera->getView() * mainCamera->getProjection());
		bloomShader->CommitChange();

		bloomShader->beginPass(0);
		mBill->render();
		bloomShader->end();

		if (0) {
			//if (defered) {
			applyDeferedRendering(q);
		}
		else {
			//mActiveShader = normalMapShader;
			mActiveShader = shadowShader;

			if (mActiveShader == shadowShader)
			{
				mActiveLight = mDefaultDirectionLight;
				if (mActiveLight->isCastShadow())
				{
					mActiveLight->getShadow()->getShadowMap()->bindAsRenderTarget(0);

					mActiveShader->begin("RenderShadow");
					mActiveShader->beginPass(0);
					for (u32 i = 0; i < q->size(); ++i)
					{
						(*q)[i]->render(mActiveShader, mainCamera, this);
					}
					mActiveShader->endPass();
					mActiveShader->end();


					

					applyGaussBlur(512, 512, mActiveLight->getShadow()->getShadowMap());



					// second pass 
					RenderingEngine::getInstancePtr()->getRenderTarget(0)->bindAsRenderTarget(0, false, true);
					mActiveShader->begin("RenderScene");
					mActiveShader->beginPass(0);

				}
			}
			else if (mActiveShader == normalMapShader) {
				mActiveShader->begin("normalMapPointLight");
				mActiveShader->beginPass(0);

			}
			else if (mActiveShader == defaultShader) {
				defaultShader->begin("defaultLighting");
				mActiveShader->beginPass(0);

			}

			Shader *alter = sphereMap;

			if (!alter) {
				for (u32 i = 0; i < q->size(); ++i) {
					(*q)[i]->render(mActiveShader, mainCamera, this);
				}
				mActiveShader->end();
			}

			if (alter)
			{
				u32 i = 0;
				for (; i < q->size() - 1; ++i) {
					(*q)[i]->render(mActiveShader, mainCamera, this);
				}
				GameObject *alterobj = (*q)[i];
				mActiveShader->endPass();
				mActiveShader->end();

				if (alter == transparentShader) {
					transparentShader->begin("TransparentLighting");
					transparentShader->beginPass(0);
					alterobj->render(transparentShader, mainCamera, this);
					transparentShader->endPass();
					transparentShader->end();
				}
				else if (alter == sphereMap)
				{
					Quaternion invRot = mSphereMirror->getMirrorRotation(mainCamera->getRot());
					Camera alterCamera(alterobj->getPos(), invRot, pi / 4, 800, 600, 1.0, 3000);

					// 球体内部相机和主相机方向相反，做一次RTT
					mSphereMirror->getEnvMap()->bindAsRenderTarget(0);

					applySky(mSky, skyShader, &alterCamera);


					defaultShader->begin("defaultLighting");
					defaultShader->beginPass(0);
					for (int j = 0; j < q->size() - 1; ++j) {
						(*q)[j]->render(defaultShader, &alterCamera, this);
					}
					defaultShader->endPass();
					defaultShader->end();

					//RECT rc = { 0, 0, 300, 300 };
					//RenderingEngine::getInstancePtr()->getRenderTarget(0)->copy(mSphereMirror->getMirror(), rc);

					RenderingEngine::getInstancePtr()->getRenderTarget(0)->bindAsRenderTarget(0, false, false);
					sphereMap->begin("spheremapLighting");
					alter->setValue("MRF", mSphereMirror->getReflectTransform(mainCamera->getRot()));
					alter->setValue("t_mirror", mSphereMirror->getEnvMap());
					sphereMap->beginPass(0);
					alterobj->render(sphereMap, mainCamera, this);
					sphereMap->endPass();
					sphereMap->end();

				}

			}

			
		
		}


		/// fxaa
		//applyFXAA();

		
	}


	void SceneManager::applyFilterDraw(int w, int h, Texture*r)
	{
		if(r)
			r->bindAsRenderTarget(0);
		vertex2D vtx[] = {
			vertex2D(0 - 0.5, 0 - 0.5, 0, 0, 0),
			vertex2D(w - 0.5, h - 0.5, 0, 1.0, 1.0),
			vertex2D(0 - 0.5, h - 0.5, 0, 0, 1.0),

			vertex2D(0 - 0.5, 0 - 0.5, 0, 0, 0),
			vertex2D(w - 0.5, 0 - 0.5, 0, 1.0, 0),
			vertex2D(w - 0.5, h - 0.5, 0, 1.0, 1.0),
		};
	
		RenderingEngine::getInstancePtr()->drawImg2D((void*)vtx, 2);

	}

	void SceneManager::_applyGaussBlur(int w, int h, vector3f filterScale, Texture*s, Texture *t)
	{
		gaussBlurShader->setValue("t_texture", s);
		gaussBlurShader->setValue("blurScale", filterScale);
		gaussBlurShader->begin("GaussBlur");
		gaussBlurShader->beginPass(0);
		applyFilterDraw(w, h, t);
		gaussBlurShader->endPass();
		gaussBlurShader->end();
	}

	void SceneManager::applyGaussBlur(int w, int h, Texture *img)
	{
		if (Input->KeyDown(DIK_SPACE))
			img->savetofile("b.jpg");
		_applyGaussBlur(w, h, vector3f(0, 1.0 / w, 0), img, mR_ShadowFilter);
		_applyGaussBlur(w, h, vector3f(1.0 / h, 0, 0), mR_ShadowFilter, img);

		if (Input->KeyDown(DIK_SPACE))
			img->savetofile("a.jpg");
	}


	void SceneManager::applyDeferedRendering(vector<GameObject*> *q)
	{


		// set RT
		for (u32 i = 0; i < df_N; ++i)
			mDeferedRT[i]->bindAsRenderTarget(i);
		//RenderingEngine::getInstancePtr()->clearColorBuffer();
		//RenderingEngine::getInstancePtr()->clearZBuffer();

		int npass = defered->begin("DeferedShading");
		defered->beginPass(0);

		for (int k = 0; k < q->size(); ++k) {
			(*q)[k]->render(defered, mainCamera, this);
		}
		defered->endPass();

		//D3DXSaveTextureToFile("1.jpg", D3DXIFF_JPG, dynamic_cast<d3d9Texture*>(mDeferedRT[df_diffuse])->getInstance(), 0);
		//D3DXSaveTextureToFile("2.jpg", D3DXIFF_JPG, dynamic_cast<d3d9Texture*>(mDeferedRT[df_normal])->getInstance(), 0);
		//D3DXSaveTextureToFile("3.jpg", D3DXIFF_JPG, dynamic_cast<d3d9Texture*>(mDeferedRT[df_depth])->getInstance(), 0);

		// second pass
		defered->beginPass(1);
		RenderingEngine::getInstancePtr()
			->getRenderTarget(0)
			->bindAsRenderTarget(0);

		
		defered->setValue("t_diffuse", mDeferedRT[df_diffuse]);
		defered->setValue("t_normal", mDeferedRT[df_normal]);
		defered->setValue("t_depth", mDeferedRT[df_depth]);

		float w = 800, h = 600;
		vertex2D vtx[] = {
			vertex2D(0, 0, 0, 0, 0),
			vertex2D(w, h, 0, 1.0, 1.0),
			vertex2D(0, h, 0, 0, 1.0),

			vertex2D(0, 0, 0, 0, 0),
			vertex2D(w, 0, 0, 1.0, 0),
			vertex2D(w, h, 0, 1.0, 1.0),
		};

		RenderingEngine::getInstancePtr()->drawImg2D((void*)vtx, 2);

		defered->endPass();

		defered->end();

	}



	void SceneManager::applySky(GameObject *sky, Shader *shader, Camera *cam)
	{
		if (skyShader)
		{
			mSky->setPos(cam->getPos())
				->setScale(vector3f(10, 10, 10))
				->updateTransform();
			skyShader->begin("SkySphere");
			skyShader->beginPass(0);
			skyShader->getInstance()->SetTexture("t_sky", mEnvMap);

			mSky->render(skyShader, cam, this);
			skyShader->end();
		}
	}

	void SceneManager::applyFXAA()
	{
		//RenderingEngine::getInstancePtr()->clearZBuffer();
		Texture *back = RenderingEngine::getInstancePtr()->getRenderTarget(0);
		RECT rc = { 0, 0, 800, 600 };
		m_Rpostpocess->copy(back, rc);
		fxaa->setValue("t_texture", m_Rpostpocess);

		m_Rpostpocess1->bindAsRenderTarget(0);


		float w = 800, h = 600;
		vertex2D vtx[] = {
			vertex2D(0, 0, 0, 0, 0),
			vertex2D(w, h, 0, 1.0, 1.0),
			vertex2D(0, h, 0, 0, 1.0),

			vertex2D(0, 0, 0, 0, 0),
			vertex2D(w, 0, 0, 1.0, 0),
			vertex2D(w, h, 0, 1.0, 1.0),
		};

		fxaa->begin("FXAA");
		fxaa->beginPass(0);
		RenderingEngine::getInstancePtr()->drawImg2D((void*)vtx, 2);
		fxaa->endPass();
		fxaa->end();

		
		//applyFilter(gaussBlurShader, vector3f(0, 1.0 / 800, 0), m_Rpostpocess1, m_Rpostpocess);
		//applyFilter(gaussBlurShader, vector3f(1.0 / 600, 0, 0), m_Rpostpocess, m_Rpostpocess1);


		back->copy(m_Rpostpocess1, rc);

		//m_Rpostpocess->savetofile("a.jpg");
		if (Input->KeyDown(DIK_SPACE))
			m_Rpostpocess1->savetofile("c.jpg");

		back->bindAsRenderTarget(0);

	}

}
