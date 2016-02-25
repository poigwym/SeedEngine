#pragma once 

#include"Mesh.h"


#include"d3d9RenderingEngine.h"

namespace Seed
{
	class OBJmesh
	{
	public:
		map<string, string> meshToFile;
		map<string, int> stringToInt;
		map<int, string> intToString;

		OBJmesh() {
			meshToFile.clear();
		}

		struct Pos
		{
			float x, y, z;
		};

		struct Tex{
			float u, v;
		};

		struct Normal {
			float x, y, z;
		};

		struct Face {
			int iv[3], it[3], in[3];
		};

		Pos *pos;
		Tex *tex;
		Normal *normal;
		Face *face;
		int *faceBelong;
		int *groupTextureID;
		int nface;

		string filePath;
		LPDIRECT3DTEXTURE9 *texture;
		LPDIRECT3DTEXTURE9 defaultTexture;
		IDirect3DVertexDeclaration9 *pVertexDecl;


		void loadMtl(string file)
		{
			file = filePath + file;
			ifstream in(file.c_str(), ios::in);
			if (!in.is_open())
				exit(1);
			char buf[256];
			int n = 0;
			while (in.getline(buf, 256))
			{
				istringstream is(buf);
				string str;
				is >> str;
				if (str == "newmtl")
				{
					string a, b;
					is >> a >> str >> b;
					meshToFile[a] = b;		// material size
					stringToInt[a] = n;   // useMtl a  
					intToString[n] = b;
					n++;
				}
			}
			in.close();

			// create texture
			texture = new LPDIRECT3DTEXTURE9[n];
			int i = 0;
			for (map<int, string>::iterator it = intToString.begin(); it != intToString.end(); ++it)
			{
				D3DXCreateTextureFromFile(d3d9RenderingEngine::getDevice(), it->second.c_str(), &texture[i]);
				++i;
			}
			if (FAILED(D3DXCreateTextureFromFile(d3d9RenderingEngine::getDevice(), "picture/defaultTexture.png", &defaultTexture)))

				exit(1);

		}

		Mesh* parse(string file)
		{
			filePath = "";
			int i;
			for (i = (int)file.length() - 1; i >= 0 && file[i] != '/'; --i);
			if (i >= 0)  filePath = file.substr(0, i + 1);

			char buf[256];
			ifstream in(file.c_str(), ios::in);
			int groups = 0;
			int nowTextureID = 0;
			int nv = 0, nvn = 0, nvt = 0, nf = 0, ng = 0;

			while (in.getline(buf, 256))
			{
				if (buf[0] == 'v')
				{
					if (buf[1] == 't') nvt++;
					else if (buf[1] == 'n') nvn++;
					else nv++;
				}
				else if (buf[0] == 'f')
					++nf;

				else if (buf[0] == 'g')
					++ng;
			}

			pos = new Pos[nv];
			tex = new Tex[nvt];
			normal = new Normal[nvn];
			face = new Face[nf];
			faceBelong = new int[nf];
			groupTextureID = new int[ng];
			nface = nf;

			int vv = 0, defaultTexture = 0, nn = 0, ff = 0;

			string mtlname;
			in.clear();
			in.seekg(0, ios::beg);
			while (in.getline(buf, 256))
			{
				istringstream is(buf);
				string str;
				is >> str;
				if (str == "mtllib")
				{
					is >> str;
					mtlname = str;
				}
				else if (str == "usemtl")
				{
					is >> str;
					nowTextureID = stringToInt[str];
				}
				else if (str[0] == 'v')
				{
					if (str[1] == 't')
					{
						is >> tex[defaultTexture].u >> tex[defaultTexture].v;
						++defaultTexture;
					}
					else if (str[1] == 'n')
					{
						is >> normal[nn].x >> normal[nn].y >> normal[nn].z;
						//normal[nn].y = 1 - normal[nn].y;
						++nn;
					}
					else
					{
						is >> pos[vv].x >> pos[vv].y >> pos[vv].z;
						vv++;
					}
				}
				else if (str[0] == 'f')
				{

					for (int i = 0; i < 3; ++i)
					{
						char format[128];
						is >> format;
						sscanf(format, "%d/%d/%d", &face[ff].iv[i], &face[ff].it[i], &face[ff].in[i]);
					}
					faceBelong[ff] = groups;

					++ff;
				}
				else if (str[0] == 'g')
				{
					is >> str;
					groupTextureID[groups] = nowTextureID;   //同一组用一个纹理id
					groups++;
				}
			}
			in.close();
			//loadMtl(mtlname);

			Mesh *mesh = new Mesh;
			for (int i = 0; i < nface; ++i)
			{
				vertexNT v[3];
				for (int j = 0; j < 3; ++j)
					v[j] = vertexNT(pos[face[i].iv[j] - 1].x, pos[face[i].iv[j] - 1].y, pos[face[i].iv[j] - 1].z,
					normal[face[i].in[j] - 1].x, normal[face[i].in[j] - 1].y, normal[face[i].in[j] - 1].z,
					tex[face[i].it[j] - 1].u, tex[face[i].it[j] - 1].v);

				calTangent(v, v + 1, v + 2);
				mesh->addFace(v[0], v[1], v[2]);
			}
			mesh->build();

			delete pos;
			delete  normal;
			delete tex;
			delete face;
			delete faceBelong;
			delete groupTextureID;

			return mesh;

		}


		virtual ~OBJmesh() {
			/*delete[]pos;
			delete[]tex;
			delete[]face;
			delete[]normal;
			delete[]faceBelong;
			delete[]groupTextureID;*/
		}

	};
}