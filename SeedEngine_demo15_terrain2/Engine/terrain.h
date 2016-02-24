#pragma once
#include"GameObject.h"

namespace Seed
{

	class TerrainPatch 
	{
		friend class GeoMipmap;

		GeoMipmap *parent;

		VertexBuffer *vb;
		IndexBuffer *ib;
		vertexCom *mvtx;
		short *pIndex;
		int nVertex, nIndex, nTriangle;

		int lod, patchsize;
	public:

		TerrainPatch(GeoMipmap *parent, int patchsize);
		virtual ~TerrainPatch() {
			DELETE(vb);
			DELETE(ib);
			DELETE(mvtx);
		}

		inline bool detailer(const TerrainPatch* adj) const {
			return adj == NULL || lod < adj->lod;
		}

		void generatePatch(float x0, float z0, bool lb, bool rb, bool ub, bool db);
		void initBuffer(float x0, float z0, bool lb, bool rb, bool ub, bool db);
		float getHeight(int x, int z);		// H(x, z) in heightmap
		void  renderPatch();
		
	};

	class GeoMipmap : public GameObject
	{
		int mMapsize;
		int nPatch, mPatchSize;							// nPatch = mMapsize / mPatchSize
		int mMaxLOD;
		vector<float> heightmap;
		vector<TerrainPatch*> mPatches;
	public:
		// patchsize formed by 2^n 
		// heightmap.size = (mapsize+1)*(mapsize+1)  e.p  mapsize=256 -> heightmap = 257x257
		GeoMipmap(int mapsize, int patchsize);		/// 256 for 256x256,  32 for 32x32
		virtual ~GeoMipmap();

		inline TerrainPatch* patch(int i, int j) {
			if (i < 0 || j < 0 || i >= nPatch || j >= nPatch) return 0;
			return mPatches[i*nPatch + j];
		}

		inline float getHeight(int x, int z) {
			assert(x*mMapsize + z < heightmap.size());
			return heightmap[x*(1+mMapsize) + z];
		}

		void _render(Camera *cam, int depth);
	};

}