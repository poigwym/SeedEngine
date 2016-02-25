#include"d3d9Headers.h"
#include"CoreHeaders.h"


namespace Seed
{

	Shader::Shader(const string &file) {
		ID3DXBuffer *errorbuf;
		if (FAILED(D3DXCreateEffectFromFile(d3d9RenderingEngine::getDevice(), file.c_str(), 0, 0, 0, 0, &es, &errorbuf)))
			throw((char*)errorbuf->GetBufferPointer());




		mData = new ShaderData;
	}

	void Shader::setValue(const string& name, Texture* tex)
	{
		if(tex)
			es->SetTexture(name.c_str(), dynamic_cast<d3d9Texture*>(tex)->getInstance());
	}



	void Shader::update(GameObject *obj, Camera *cam, SceneManager *smgr)
	{

		// setWorld
		setValue("MW", obj->getTransform());

		setValue("tex_color", obj->getTexture());
		setValue("normalMap", obj->getNormalMap());


		setValue("camPos", cam->getPos());
		// setView
		setValue("MV", cam->getView());
		// setProj
		setValue("MP", cam->getProjection());
		setValue("MVP", cam->getView() * cam->getProjection());

		setValue("pointlight", smgr->getPointLight());
		setValue("directionlight", smgr->getDirctionLight());


		//DirectionLight *dir = smgr->getDirctionLight();

		Light* activeLight = smgr->getActiveLight();
		/*if (activeLight->isCastShadow()) {
			Shadow *shadow = activeLight->getShadow();
			setValue("MLVP", activeLight->calShadowTransform());
			setValue("t_shadowMap",
				dynamic_cast<d3d9Texture*>(activeLight->getShadow()->getShadowMap())
				->getInstance());
		}*/

		// setData from the dataMap
		for (auto i = mData->getFMap().begin(); i != mData->getFMap().end(); ++i) {
			setValue(i->first, i->second);
		}
		for (auto i = mData->getVec3Map().begin(); i != mData->getVec3Map().end(); ++i) {
			setValue(i->first, i->second);
		}
		for (auto i = mData->getMatMap().begin(); i != mData->getMatMap().end(); ++i) {
			setValue(i->first, i->second);
		}

		es->CommitChanges();
	}



}