#include"GameObject.h"
#include"Shader.h"


namespace Seed
{

	 void GameObject::postRender(Shader* shader, Camera *cam, SceneManager* smgr)
	{
		 for (int i = 0; i < mChilds.size(); ++i)
		 {
			 dynamic_cast<GameObject*>(mChilds[i])->render(shader, cam, smgr);
		 }
	}

	void GameObject::render(Shader* shader, Camera *cam, SceneManager* smgr)
	{
		_render(shader, cam, smgr);
		postRender(shader, cam, smgr);

	}

	void GameObject::_render(Shader* shader, Camera *cam, SceneManager* smgr)
	{
		// set shader variable and draw mesh
		if (mesh) {
			shader->update(this, cam, smgr);
			mesh->draw();
		}
	}

	void GameObject::_render(Camera *cam, int depth)
	{
		// set shader variable and draw mesh
		if (mesh) {
			Log << "GameObject::_render" << endl;
			if (mtl) {
				Log << "begin apply material" << endl;

				mtl->applyMaterial(this, cam);
			}
			Log << "begin draw" << endl;
			mesh->draw();
		}

		for (int i = 0; i < mChilds.size(); ++i)
			dynamic_cast<GameObject*>(mChilds[i])->_render(cam, depth+1);
	}

	void GameObject::renderShadow(Material *shadowmtl)
	{
		if (mesh) {
			Log << "begin apply shadow material" << endl;
			shadowmtl->applyMaterial(this, NULL, false, false);
			Log << "begin draw to shadow map" << endl;
			mesh->draw();
		}
		for (int i = 0; i < mChilds.size(); ++i) {
			dynamic_cast<GameObject*>(mChilds[i])->renderShadow(shadowmtl);
		}
	}


}