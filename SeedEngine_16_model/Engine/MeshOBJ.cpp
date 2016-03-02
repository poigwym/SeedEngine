
#include"MeshOBJ.h"
#include"SubMesh.h"
#include"RenderingEngine.h"
#include"Geometry.h"

#include"Bug.h"

namespace Seed
{


	void MeshOBJ::parse(const string &file, SubMesh *mesh)
	{
		Geometry *geo = new Geometry;
		parse(file, geo);

		mesh->setGeometry(geo);
	}



	void MeshOBJ::parse(const string &file, Geometry* geo)
	{
		vector<Vec3> pos;
		vector<Vec2> uv;
		vector<Vec3> nor;


		FILE *in = fopen(file.c_str(), "r");
		if (!in) throw("MeshOBJ :: parse --- open file failed ");
		Log(" parse obj begin");


		bool useTex = false, useNor = false;
		char c;
		char buf[256];



		while ((c = fgetc(in)) != EOF)
		{
			if (c == 'v')
			{
				c = fgetc(in);			// v vt vn
				if (c == 't')
				{
					// tex
					Vec2 tex;
					fgets(buf, 256, in);
					sscanf(buf, " %f %f", &tex.x, &tex.y);
					uv.push_back(tex);
					useTex = true;
				}
				else if (c == 'n')
				{
					// normal
					Vec3 normal;
					fgets(buf, 256, in);
					sscanf(buf, " %f %f %f", &normal.x, &normal.y, &normal.z);
					nor.push_back(normal);
					useNor = true;
				}
				else if (c == ' ' || c == '\t')
				{
					// pos
					Vec3 p;
					fgets(buf, 256, in);
					sscanf(buf, " %f %f %f", &p.x, &p.y, &p.z);
					pos.push_back(p);
				}
				else
					fgets(buf, 256, in);
			}
		}
		

		VertexDeclaration *decl = RenderingEngine::getInstance().createVertexDeclaration();
		decl->addElementSemantic(VES_POSITION);
		if (useTex)
			decl->addElementSemantic(VES_TEXCOORD);
		if (useNor)
			decl->addElementSemantic(VES_NORMAL);

		geo->setVertexDeclaration(decl);

		fseek(in, 0, SEEK_SET);
		while ((c = fgetc(in)) != EOF)
		{
			if (c == 'f')
			{
				//  face
				Vertex v[3];

				for (int i = 0; i < 3; ++i)
				{

					// pos/tex/normal
					if (useTex && useNor)
					{
						int ip, it, inor;
						fscanf(in, " %d/%d/%d", &ip, &it, &inor);

						//assert(ip <= pos.size());
						//assert(it <= uv.size());
						//assert(inor <= nor.size());

						if (ip <= pos.size() &&
							it <= uv.size() &&
							inor <= nor.size())
						{

							v[i].addPosition( pos[ip - 1].x, pos[ip - 1].y, pos[ip - 1].z);
							v[i].addTexcoord(uv[it - 1].x, uv[it - 1].y);
							v[i].addNormal(nor[inor - 1].x, nor[inor - 1].y, nor[inor - 1].z);
						}
					}

					// pos/tex
					else if (useTex)
					{
						int ip, it;
						fscanf(in, " %d/%d", &ip, &it);

						assert(ip <= pos.size());
						assert(it <= uv.size());
						v[i].addPosition(pos[ip - 1].x, pos[ip - 1].y, pos[ip - 1].z);
						v[i].addTexcoord(uv[it - 1].x, uv[it - 1].y);
					}

					// pos//normal
					else if (useTex && useNor)
					{
						int ip, inor;
						fscanf(in, " %d//%d", &ip, &inor);
						assert(ip <= pos.size());
						assert(inor <= nor.size());
						v[i].addPosition(pos[ip - 1].x, pos[ip - 1].y, pos[ip - 1].z);
						v[i].addNormal(nor[inor - 1].x, nor[inor - 1].y, nor[inor - 1].z);
					}
				}
				fgets(buf, 256, in);
				geo->addTriangle(v[0], v[1], v[2]);
			}
			else 
				fgets(buf, 256, in);

		}

		fclose(in);
		Log(" parse obj end");
	}

}