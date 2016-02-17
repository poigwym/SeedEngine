#include"GameObject.h"
#include"Shader.h"



namespace NBengine
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

}