#pragma once


#include"GameObject.h"
#include"RenderingEngine.h"
#include"Geometry.h"
namespace Seed
{

	class AA : public GameObject
	{
		Geometry *gg;
	public:
		AA()

		{
			VertexDeclaration *decl = RenderingEngine::getInstance().createVertexDeclaration();
			decl->addElementSemantic(VES_POSITION);
			decl->addElementSemantic(VES_NORMAL);
			decl->addElementSemantic(VES_TEXCOORD);
			gg = new Geometry();
			gg->setVertexDeclaration(decl);//

			Vertex v[3];


			v[0].addPosition(0, -2, 5);
			v[0].addNormal(0, 1, 0);
			v[0].addTexcoord(0, 0);


			v[1].addPosition(5, -2, 0);
			v[1].addNormal(0, 1, 0);
			v[1].addTexcoord(1, 1);

			v[2].addPosition(0, -2, 0);
			v[2].addNormal(0, 1, 0);
			v[2].addTexcoord(0, 1);


			gg->addTriangle(v[0], v[1], v[2]);//
		}
		void _render(Camera *cam, int depth)
		{

			/*if (mtl) {
				Log("begin apply material");

				mtl->applyMaterial(this, cam);
			}*/

			RenderingEngine::getInstance().renderGeometry(gg);
		}
	};

}