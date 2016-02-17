#pragma once

#include"Transform.h"
#include"Mesh.h"
#include"prequest.h"

namespace NBengine
{



	// ÊµÌåÊ÷
	class GameObject : public Transform
	{
	protected:
		Mesh *mesh;
		Texture *tex;
		Texture *normalMap;

	public:
		GameObject() {
			mesh = 0;
			tex = 0;
			normalMap = 0;
		}

		virtual ~GameObject(){ }


		virtual inline GameObject* addChild(GameObject *c){ return dynamic_cast<GameObject*>(Transform::addChild(c)); }

		virtual inline GameObject* setPos(const vector3f &pos) { return dynamic_cast<GameObject*>(Transform::setPos(pos)); }
		virtual inline GameObject* setRot(const Quaternion &rot) { return dynamic_cast<GameObject*>(Transform::setRot(rot)); }
		virtual inline GameObject* setScale(const vector3f &scale) { return dynamic_cast<GameObject*>(Transform::setScale(scale)); }

		virtual void input() {
			Transform::input();

			//extend
		}

		virtual void update()
		{
			Transform::update();
			
			// extend
		}



		virtual void postRender(Shader* shader, Camera *cam, SceneManager*renderingEngine);
		virtual void render(Shader* shader, Camera *ca, SceneManager*renderingEngine); 
		virtual void _render(Shader* shader, Camera *cam, SceneManager*renderingEngine);

		inline GameObject* addMesh(Mesh *mesh) {
			this->mesh = mesh;
			return this;
		}

		inline GameObject* addTexture(Texture *tex) {
			this->tex = tex;
			return this;
		}
		inline  Texture* getTexture() { return tex; }

		inline GameObject* addNormalMap(Texture *normalMap) {
			this->normalMap = normalMap;
			return this;
		}
		inline  Texture* getNormalMap() { return normalMap; }
	};

}