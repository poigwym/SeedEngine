#pragma once

#include"GameObject.h"
#include"math3d.h"
#include"Geometry.h"



#include"Bug.h"

namespace Seed
{

	class Bone : public GameObject
	{

	protected:
		matrix4f MComb[4];
		Geometry *gg;
	public:
		Bone()
		{
			MComb[0].initTranslate(0, 0, 0);
			MComb[1].initTranslate(0, 1, 1);



			VertexDeclaration *decl = RenderingEngine::getInstance().createVertexDeclaration();
			decl->addElementSemantic(VES_POSITION);
			decl->addElementSemantic(VES_NORMAL);
			decl->addElementSemantic(VES_TEXCOORD);
			decl->addElementSemantic(VES_BLEND_INDICES);
			decl->addElementSemantic(VES_BLEND_WEIGHTS);
			gg = new Geometry();
			gg->setVertexDeclaration(decl);//

			Vertex v[3];


			v[0].addPosition(0, -2, 5);
			v[0].addNormal(0, 1, 0);
			v[0].addTexcoord(0, 0);
			v[0].addBlendIndices(0,1);
			v[0].addBlendWeights(0.5,0.5);


			v[1].addPosition(5, -2, 0);
			v[1].addNormal(0, 1, 0);
			v[1].addTexcoord(1, 1);
			v[1].addBlendIndices(0, 1);
			v[1].addBlendWeights(1, 0);

			v[2].addPosition(0, -2, 0);
			v[2].addNormal(0, 1, 0);
			v[2].addTexcoord(0, 1);
			v[2].addBlendIndices(0, 1);
			v[2].addBlendWeights(1, 0);


			gg->addTriangle(v[0], v[1], v[2]);//
		}
		void _render(Camera *cam, int depth)
		{
			if (gg)
				RenderingEngine::getInstance().renderGeometry(gg);
		}

		matrix4f* getBoneMatrix() {
			return MComb
				;
		}

		void update()
		{
			MComb[1] = MComb[1] * matrix4f().initRotateQuaternion(
				Quaternion(vector3f(0, 1, 0), pi*0.001));

			updateTransform();
		}

	};
}