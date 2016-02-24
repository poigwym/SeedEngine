#include"terrain.h"
#include"d3d9RenderingEngine.h"
#include"Material.h"
#include"camera.h"
#include"d3d9VertexIndexBuffer.h"

namespace Seed
{
	GeoMipmap::GeoMipmap(int mapsize, int patchsize) :mMapsize(mapsize), mPatchSize(patchsize)
	{
		nPatch = mapsize / patchsize;
		mMaxLOD = log(patchsize) / log(2);

		heightmap.resize((mapsize + 1)*(mapsize + 1));
		int mx = mapsize / 2;
		int i, j;
		for (i = 0; i <= mapsize; ++i)
		for (j = 0; j <= mapsize; ++j)
			heightmap[i*(mapsize + 1) + j] = 
			5 - 0.3*sqrt(abs(i - mx)*abs(i - mx) + abs(j - mx)*abs(j - mx));


		Log << "init patches begin" << endl;

		mPatches.resize(nPatch*nPatch);
		for (i = 0; i < mPatches.size(); ++i) {
			mPatches[i] = new TerrainPatch(this, patchsize);
			//mPatches[i]->lod = 0;
			mPatches[i]->lod = rand()%mMaxLOD;
		}

		for(i=0;i<nPatch;++i) 
		for (j = 0; j < nPatch; ++j)
		{
			TerrainPatch *pch = patch(i, j);
			pch->generatePatch(i*mPatchSize, j*mPatchSize,
				//1, 1, 1, 1)
				pch->detailer(patch(i, j - 1)),
				//1,
				pch->detailer(patch(i, j + 1)),
				//1,
				pch->detailer(patch(i - 1, j)),
				//1,
				pch->detailer(patch(i + 1, j))
				//1
				)
				;

			Log << i << ' ' << j << ' ' << "triangle" << pch->nTriangle << endl;
		}



		Log << "init patches end" << endl;
	}


	GeoMipmap:: ~GeoMipmap()
	{
		for (int i = 0; i < mPatches.size(); ++i) {
			DELETE(mPatches[i]);
		}
	}

	void GeoMipmap::_render(Camera *cam, int depth)
	{
		// set shader variable and draw mesh
		Log << "GeoMipmap::_render" << endl;
		if (mtl) {
			Log << "begin apply material" << endl;

			mtl->applyMaterial(this, cam);
		}
		Log << " draw terrain begin" << endl;
		for (int i = 0; i<nPatch; ++i)
		for (int j = 0; j < nPatch; ++j)
			patch(i,j)->renderPatch();
		//patch(0,0)->renderPatch();

		if (mtl)
			mtl->fireMaterial();
		Log << " draw terrain end" << endl;

	}




	TerrainPatch::TerrainPatch(GeoMipmap *parent, int patchsize) : parent(parent),  patchsize(patchsize)
	{
		vb = 0;
		ib = 0;
		mvtx = 0;
		pIndex = 0;
		nVertex = 0;
		nIndex = 0;
		nTriangle = 0;
		lod = 0;

	}

	float TerrainPatch::getHeight(int x, int z)
	{
		return parent->getHeight(x, z);
	}

	void TerrainPatch::initBuffer(float x0, float z0, bool lb, bool rb, bool ub, bool db)
	{
		DELETE(vb);
		DELETE(ib);
		DELETE(mvtx);
		// 一个patch分div块
		int div = (patchsize >> lod);
		if (div > 1)
		{
			
			float perlen = (float)patchsize / div;		// 每块的大小
			float x, z;
			nIndex = 0;
			nVertex = 0;
			nTriangle = 0;

			// left edge
			if (lb) {
				for (x = x0; x < x0 + patchsize; x += perlen * 2)
				{
					// left edge
					nVertex += 3; nIndex += 3;  ++nTriangle;
					// left up
					if (x != x0) { nVertex += 3; nIndex += 3; ++nTriangle; }
					// left down
					if (x + 2 * perlen != x0 + patchsize) { nVertex += 3; nIndex += 3;  ++nTriangle; }
				}
			}
			else {
				for (x = x0 + perlen; x < x0 + patchsize - perlen; x += perlen) {
					nVertex += 4; nIndex += 6; nTriangle += 2;
				}
				// add 2 triangle to make left  trapezoid
				nVertex += 3; nIndex += 3; ++nTriangle;
				nVertex += 3; nIndex += 3; ++nTriangle;
			}


			// right edge
			if (rb) {
				for (x = x0; x < x0 + patchsize; x += perlen * 2)
				{
					// right edge
					nVertex += 3; nIndex += 3; ++nTriangle;
					//  right up
					if (x != x0) { nVertex += 3; nIndex += 3; ++nTriangle; }
					// right down
					if (x + 2 * perlen != x0 + patchsize) { nVertex += 3; nIndex += 3; ++nTriangle; }
				}
			}
			else  {
				for ( x = x0 + perlen; x < x0 + patchsize - perlen; x += perlen) {
					nVertex += 4; nIndex += 6; nTriangle += 2;
				}
				// add 2 triangle to make right  trapezoid
				nVertex += 3; nIndex += 3; ++nTriangle;
				nVertex += 3; nIndex += 3; ++nTriangle;
			}


			// up edge 
			if (ub) {
				for (z = z0; z < z0 + patchsize; z += 2 * perlen)
				{
					// up edge
					nVertex += 3; nIndex += 3; ++nTriangle;
					//  up left
					if (z != z0) { nVertex += 3; nIndex += 3; ++nTriangle; }
					// up right
					if (z + 2 * perlen != z0 + patchsize) { nVertex += 3; nIndex += 3;  ++nTriangle; }
				}
			}
			else {
				for ( z = z0 + perlen; z < z0 + patchsize - perlen; z += perlen) { 
					nVertex += 4; nIndex += 6; nTriangle += 2;
				}
				// add 2 triangle to make up trapezoid
				nVertex += 3; nIndex += 3; ++nTriangle;
				nVertex += 3; nIndex += 3; ++nTriangle;
			}

			 //down edge
			if (db) {
				for (z = z0; z < z0 + patchsize; z += 2 * perlen)
				{
					// down edge
					nVertex += 3; nIndex += 3; ++nTriangle;
					// left down
					if (z != z0) { nVertex += 3; nIndex += 3; ++nTriangle; }
					// right down
					if (z + 2 * perlen != z0 + patchsize) { nVertex += 3; nIndex += 3; ++nTriangle; }
				}
			}
			else {
				for (z = z0 + perlen; z < z0 + patchsize - perlen; z += perlen) {
					nVertex += 4; nIndex += 6; nTriangle += 2;
				}
				// add 2 triangle to make down trapezoid
				nVertex += 3; nIndex += 3; ++nTriangle;
				nVertex += 3; nIndex += 3; ++nTriangle;
			}

			// midle
			for (x = x0 + perlen; x < x0 + patchsize - perlen; x += perlen)
			for (z = z0 + perlen; z < z0 + patchsize - perlen; z += perlen)
			{
				nVertex += 4; nIndex += 6; nTriangle += 2;
			}

			mvtx = new vertexCom[nVertex];
			pIndex = new short[nIndex];
			vb = RenderingEngine::getInstance().createVertexbuffer(nVertex, fvf_common);
			ib = RenderingEngine::getInstance().createIndexbuffer16(nIndex);
		}
		else
		{
			mvtx = new vertexCom[nVertex=4];
			pIndex = new short[nIndex=6];
			nTriangle = 2;
			vb = RenderingEngine::getInstance().createVertexbuffer(nVertex, fvf_common);
			ib = RenderingEngine::getInstance().createIndexbuffer16(nIndex);
		}
	}

	void TerrainPatch::generatePatch(float x0, float z0, bool lb, bool rb, bool ub, bool db)
	{
		
		initBuffer(x0, z0, lb, rb, ub, db);
		int div = patchsize >> lod;
		float perlen = (float)patchsize / div;
		if (div > 1)
		{
			float x, z;
			int nv = 0, ni = 0;

			// left edge
			if (lb) {
				for (x = x0; x < x0 + patchsize; x += perlen * 2)
				{
					// left edge
					mvtx[nv] = vertexCom(x, getHeight(x, z0), z0, 0, 1, 0, 0, 0);
					mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 0.5);
					mvtx[nv + 2] = vertexCom(x + 2 * perlen, getHeight(x + 2 * perlen, z0), z0, 0, 1, 0, 0, 1);

					pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
					nv += 3; ni += 3;

					// left up
					if (x != x0)
					{
						mvtx[nv] = vertexCom(x, getHeight(x, z0), z0, 0, 1, 0, 0, 0);
						mvtx[nv+1] = vertexCom(x, getHeight(x, z0+perlen), z0+perlen, 0, 1, 0, 1, 0);
						mvtx[nv + 2] = vertexCom(x+perlen, getHeight(x+perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 1);
						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
					// left down
					if (x + 2 * perlen != x0 + patchsize)
					{
						mvtx[nv] = vertexCom(x + 2 * perlen, getHeight(x + 2 * perlen, z0), z0, 0, 1, 0, 0, 1);
						mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 0.5);
						mvtx[nv + 2] = vertexCom(x + 2 * perlen, getHeight(x + 2 * perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 1);
						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
				}
			} else {
				for (z=z0, x = x0 + perlen; x < x0 + patchsize - perlen; x += perlen)
				{
					mvtx[nv] = vertexCom(x, getHeight(x, z), z, 0, 1, 0, 0, 0);
					mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z), z, 0, 1, 0, 0, 1);
					mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z + perlen), z + perlen, 0, 1, 0, 1, 1);
					mvtx[nv + 3] = vertexCom(x, getHeight(x, z + perlen), z + perlen, 0, 1, 0, 1, 0);

					pIndex[ni] = nv; pIndex[ni + 1] = nv + 2; pIndex[ni + 2] = nv + 1;
					pIndex[ni + 3] = nv; pIndex[ni + 4] = nv + 3; pIndex[ni + 5] = nv + 2;

					nv += 4; ni += 6;
				}
				// add 2 triangle to make left  trapezoid
				mvtx[nv] = vertexCom(x0, getHeight(x0, z0), z0, 0, 1, 0, 0, 0);
				mvtx[nv + 1] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 1);
				mvtx[nv + 2] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z0), z0, 0, 1, 0, 0, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;

				mvtx[nv] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z0), z0, 0, 1, 0, 0, 0);
				mvtx[nv + 1] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 0);
				mvtx[nv + 2] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z0), z0, 0, 1, 0, 0, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;
			}


			// right edge
			if (rb) {
				for (x = x0; x < x0 + patchsize; x += perlen * 2)
				{
					// right edge
					mvtx[nv] = vertexCom(x, getHeight(x, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 0);
					mvtx[nv + 1] = vertexCom(x + 2 * perlen, getHeight(x + 2 * perlen, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 1);
					mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 0.5);

					pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
					nv += 3; ni += 3;

					//  right up - left
					if (x != x0)
					{
						mvtx[nv] = vertexCom(x, getHeight(x, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 0);
						mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 0.5);
						mvtx[nv + 2] = vertexCom(x, getHeight(x, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 0);
						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
					// right down - left
					if (x + 2 * perlen != x0 + patchsize)
					{
						mvtx[nv] = vertexCom(x + 2 * perlen, getHeight(x + 2 * perlen, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 1);
						mvtx[nv + 1] = vertexCom(x + 2 * perlen, getHeight(x + 2 * perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 1);
						mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 0.5);
						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
				}
			} else  {
				for (z=z0+patchsize-perlen, x = x0 + perlen; x < x0 + patchsize - perlen; x += perlen)
				{
					mvtx[nv] = vertexCom(x, getHeight(x, z), z, 0, 1, 0, 0, 0);
					mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z), z, 0, 1, 0, 0, 1);
					mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z + perlen), z + perlen, 0, 1, 0, 1, 1);
					mvtx[nv + 3] = vertexCom(x, getHeight(x, z + perlen), z + perlen, 0, 1, 0, 1, 0);

					pIndex[ni] = nv; pIndex[ni + 1] = nv + 2; pIndex[ni + 2] = nv + 1;
					pIndex[ni + 3] = nv; pIndex[ni + 4] = nv + 3; pIndex[ni + 5] = nv + 2;
					nv += 4; ni += 6;
				}
				// add 2 triangle to make right  trapezoid
				mvtx[nv] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 1);
				mvtx[nv+1] = vertexCom(x0, getHeight(x0, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 0);
				mvtx[nv+2] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;

				mvtx[nv] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z0+patchsize-perlen), z0+patchsize-perlen, 0, 1, 0, 0, 0);
				mvtx[nv+1] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize-perlen , z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 0);
				mvtx[nv + 2] = vertexCom(x0 + patchsize , getHeight(x0 + patchsize, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;
			}


			// up edge 
			if (ub) {
				for (z = z0; z < z0 + patchsize; z += 2 * perlen)
				{
					// up edge
					mvtx[nv] = vertexCom(x0, getHeight(x0, z), z, 0, 1, 0, 0, 0);
					mvtx[nv + 1] = vertexCom(x0, getHeight(x0, z + 2 * perlen), z + 2 * perlen, 0, 1, 0, 1, 0);
					mvtx[nv + 2] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z + perlen), z + perlen, 0, 1, 0, 0.5, 1);
					pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
					nv += 3; ni += 3;

					//  up left - left
					if (z != z0)
					{
						mvtx[nv] = vertexCom(x0, getHeight(x0, z), z, 0, 1, 0, 0, 0);
						mvtx[nv + 1] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z + perlen), z + perlen, 0, 1, 0, 0.5, 1);
						mvtx[nv + 2] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z), z, 0, 1, 0, 0, 1);

						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
					// up right -right
					if (z + 2 * perlen != z0 + patchsize)
					{
						mvtx[nv] = vertexCom(x0, getHeight(x0, z + 2 * perlen), z + 2 * perlen, 0, 1, 0, 1, 0);
						mvtx[nv + 1] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z + 2 * perlen), z + 2 * perlen, 0, 1, 0, 1, 1);
						mvtx[nv + 2] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z + perlen), z + perlen, 0, 1, 0, 0.5, 1);

						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
				}
			} else {
				for (x = x0, z = z0 + perlen; z < z0 + patchsize - perlen; z += perlen)
				{
					mvtx[nv] = vertexCom(x, getHeight(x, z), z, 0, 1, 0, 0, 0);
					mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z), z, 0, 1, 0, 0, 1);
					mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z + perlen), z + perlen, 0, 1, 0, 1, 1);
					mvtx[nv + 3] = vertexCom(x, getHeight(x, z + perlen), z + perlen, 0, 1, 0, 1, 0);

					pIndex[ni] = nv; pIndex[ni + 1] = nv + 2; pIndex[ni + 2] = nv + 1;
					pIndex[ni + 3] = nv; pIndex[ni + 4] = nv + 3; pIndex[ni + 5] = nv + 2;
					nv += 4; ni += 6;
				}
				// add 2 triangle to make up trapezoid
				mvtx[nv] = vertexCom(x0, getHeight(x0, z0), z0, 0, 1, 0, 0, 0);
				mvtx[nv+1] = vertexCom(x0, getHeight(x0, z0+perlen), z0+perlen, 0, 1, 0, 1, 0);
				mvtx[nv + 2] = vertexCom(x0 + perlen, getHeight(x0 + perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;

				mvtx[nv] = vertexCom(x0, getHeight(x0, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 0);
				mvtx[nv+1] = vertexCom(x0, getHeight(x0, z0 + patchsize), z0 + patchsize , 0, 1, 0, 1, 0);
				mvtx[nv+2] = vertexCom(x0+perlen, getHeight(x0+perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;

			}

			// down edge
			if (db) {
				for (z = z0; z < z0 + patchsize; z += 2 * perlen)
				{
					// down edge
					mvtx[nv] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z), z, 0, 1, 0, 0, 1);
					mvtx[nv + 1] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z + perlen), z + perlen, 0, 1, 0, 0.5, 0);
					mvtx[nv + 2] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z + 2 * perlen), z + 2 * perlen, 0, 1, 0, 1, 1);
					pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
					nv += 3; ni += 3;

					// left down - right
					if (z != z0)
					{
						mvtx[nv] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z), z, 0, 1, 0, 0, 0);
						mvtx[nv + 1] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z + perlen), z + perlen, 0, 1, 0, 0.5, 0);
						mvtx[nv + 2] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z), z, 0, 1, 0, 0, 1);
						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
					// right down - left
					if (z + 2 * perlen != z0 + patchsize)
					{
						mvtx[nv] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z + perlen), z + perlen, 0, 1, 0, 0.5, 0);
						mvtx[nv + 1] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z + 2 * perlen), z + 2 * perlen, 0, 1, 0, 1, 0);
						mvtx[nv + 2] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z + 2 * perlen), z + 2 * perlen, 0, 1, 0, 1, 1);
						pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
						nv += 3; ni += 3;
					}
				}
			} else {

				for (x = x0+patchsize-perlen, z = z0 + perlen; z < z0 + patchsize - perlen; z += perlen)
				{
					mvtx[nv] = vertexCom(x, getHeight(x, z), z, 0, 1, 0, 0, 0);
					mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z), z, 0, 1, 0, 0, 1);
					mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z + perlen), z + perlen, 0, 1, 0, 1, 1);
					mvtx[nv + 3] = vertexCom(x, getHeight(x, z + perlen), z + perlen, 0, 1, 0, 1, 0);

					pIndex[ni] = nv; pIndex[ni + 1] = nv + 2; pIndex[ni + 2] = nv + 1;
					pIndex[ni + 3] = nv; pIndex[ni + 4] = nv + 3; pIndex[ni + 5] = nv + 2;
					nv += 4; ni += 6;
				}
				// add 2 triangle to make down trapezoid
				mvtx[nv] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z0), z0, 0, 1, 0, 0, 1);
				mvtx[nv + 1] = vertexCom(x0 + patchsize - perlen, getHeight(x0 + patchsize - perlen, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 0);
				mvtx[nv + 2] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z0 + perlen), z0 + perlen, 0, 1, 0, 1, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;

				mvtx[nv] = vertexCom(x0+patchsize-perlen, getHeight(x0+patchsize-perlen, z0 + patchsize - perlen), z0 + patchsize - perlen, 0, 1, 0, 0, 0);
				mvtx[nv+1] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize, z0 + patchsize), z0 + patchsize, 0, 1, 0, 1, 0);
				mvtx[nv + 2] = vertexCom(x0 + patchsize, getHeight(x0 + patchsize , z0 + patchsize-perlen), z0 + patchsize-perlen, 0, 1, 0, 1, 1);
				pIndex[ni] = nv; pIndex[ni + 1] = nv + 1; pIndex[ni + 2] = nv + 2;
				nv += 3;	ni += 3;
			}

			// midle
			for (x = x0 + perlen; x< x0 + patchsize - perlen; x += perlen)
			for (z = z0 + perlen; z < z0 + patchsize - perlen; z += perlen)
			{
				// 0 3
				// 1 2
				mvtx[nv] = vertexCom(x, getHeight(x, z), z, 0, 1, 0, 0, 0);
				mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z), z, 0, 1, 0, 0, 1);
				mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z + perlen), z + perlen, 0, 1, 0, 1, 1);
				mvtx[nv + 3] = vertexCom(x, getHeight(x, z + perlen), z + perlen, 0, 1, 0, 1, 0);

				pIndex[ni] = nv;
				pIndex[ni + 1] = nv + 2;
				pIndex[ni + 2] = nv + 1;
				pIndex[ni + 3] = nv;
				pIndex[ni + 4] = nv + 3;
				pIndex[ni + 5] = nv + 2;

				nv += 4; ni += 6;
			}

			//assert(nv == nVertex);
			//assert(ni == nIndex);

			vb->update(mvtx, nVertex, fvf_common);
			ib->update(pIndex, nIndex);
			DELETEARRAY(pIndex);
		}

		else
		{
			int nv = 0, ni = 0;
			float x = x0, z = z0;
			mvtx[nv] = vertexCom(x, getHeight(x, z), z, 0, 1, 0, 0, 0);
			mvtx[nv + 1] = vertexCom(x + perlen, getHeight(x + perlen, z), z, 0, 1, 0, 0, 1);
			mvtx[nv + 2] = vertexCom(x + perlen, getHeight(x + perlen, z + perlen), z + perlen, 0, 1, 0, 1, 1);
			mvtx[nv + 3] = vertexCom(x, getHeight(x, z + perlen), z + perlen, 0, 1, 0, 1, 0);

			pIndex[ni] = nv;
			pIndex[ni + 1] = nv + 2;
			pIndex[ni + 2] = nv + 1;
			pIndex[ni + 3] = nv;
			pIndex[ni + 4] = nv + 3;
			pIndex[ni + 5] = nv + 2;

			vb->update(mvtx, nVertex, fvf_common);
			ib->update(pIndex, nIndex);
			DELETEARRAY(pIndex);
		}
	}


	void TerrainPatch::renderPatch()
	{
		Log << "renderPatch begin" << endl;
		d3d9RenderingEngine::getDevice()->SetVertexDeclaration(d3d9RenderingEngine::vertexDecl[fvf_common]);
		d3d9RenderingEngine::getDevice()->SetStreamSource(0,
			dynamic_cast<d3d9VertexBuffer*>(
			vb)->getInstance(), 0, getVertexSize(fvf_common));

		d3d9RenderingEngine::getDevice()->SetIndices(
			dynamic_cast<d3d9IndexBuffer*>(ib)->getInstance());
		d3d9RenderingEngine::getDevice()->
			DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nVertex, 0, nTriangle);
		Log << "renderPatch end" << endl;

	}

}