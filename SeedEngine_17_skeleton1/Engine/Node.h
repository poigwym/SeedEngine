#pragma once

#include"Transform.h"
#include"GameObject.h"

namespace Seed
{
	class Node
	{
	protected:
		Transform transform;
		vector<Node*> mChilds;
		vector<GameObject*> mObjects;

	public:
		Node() {}
		virtual ~Node() {}
		inline void addObject(GameObject *obj) {
			mObjects.push_back(obj);
		}

		inline virtual void update() {
			for (u32 i = 0; i < mObjects.size(); ++i)
				mObjects[i]->update();


			for (u32 i = 0; i < mChilds.size(); ++i)
				mChilds[i]->update();
		}

		inline virtual void processQueue(vector<GameObject*> *q)
		{
			for (u32 i = 0; i < mObjects.size(); ++i)
				q->push_back(mObjects[i]);

		}

	};


}
