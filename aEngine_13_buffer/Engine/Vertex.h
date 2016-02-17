#pragma once

#include"math3d.h"
#include"prequest.h"
#include"util.h"

namespace NBengine
{
	
	class vertexCom
	{
	public:
		float x, y, z;
		float nx, ny, nz;
		float u, v;

		vertexCom() :x(0), y(0), z(0), nx(0), ny(0), nz(0), u(0), v(0) {}
		vertexCom(float x, float y, float z, float nx, float ny, float nz, float u, float v) :
			x(x), y(y), z(z), nx(nx), ny(ny), nz(nz), u(u), v(v) {}
	};


	class vertex2D
	{
	public:
		float x, y, z;
		float u, v;

		vertex2D():x(0),y(0),z(0), u(0), v(0) {}
		vertex2D(float x, float y, float z, float u, float v) :
			x(x), y(y), z(z),  u(u), v(v) {}

		vertex2D(const vertex2D &v) :x(v.x), y(v.y), z(v.z), u(v.u), v(v.v) {}
	};





	class vertexNT
	{
	public:
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		float tx, ty, tz, tw;

		vertexNT():x(0),y(0),z(0),nx(0),ny(0),nz(0),u(0),v(0),tx(0),ty(0),tz(0),tw(0) {}
		vertexNT(float x, float y, float z, float nx, float ny, float nz,
			float u, float v, float tx = 0, float ty = 0, float tz = 0, float tw = 0) :x(x), y(y), z(z),
			nx(nx), ny(ny), nz(nz), u(u), v(v),
			tx(tx), ty(ty), tz(tz), tw(tw) {}
		vertexNT(const vertexNT&a) :x(a.x), y(a.y), z(a.z),
			nx(a.nx), ny(a.ny), nz(a.nz),
			u(a.u), v(a.v),
			tx(a.tx), ty(a.ty), tz(a.tz), tw(a.tw) {}
	};

	inline void calTangent(vertexNT *v1, vertexNT *v2, vertexNT *v3)
	{
		vector3f edge1(v2->x - v1->x, v2->y - v1->y, v2->z - v1->z);
		vector3f edge2(v3->x - v1->x, v3->y - v1->y, v3->z - v1->z);

		edge1=edge1.normal();
		edge2=edge2.normal();

		vector2f texEdge1(v2->u - v1->u, v2->v - v1->v);
		vector2f texEdge2(v3->u - v1->u, v3->v - v1->v);

		texEdge1=	texEdge1.normal();
		texEdge2=texEdge2.normal();

		vector3f tangent, bitangent;
		float w;
		float det = (texEdge1.getX() * texEdge2.getY()) - (texEdge1.getY() * texEdge2.getX());

		if (fabsf(det) < 1e-6f)    // almost equal to zero
		{
			tangent.setX(1.0f);
			tangent.setY(0.0f);
			tangent.setZ(0.0f);

			bitangent.setX(0.0f);
			bitangent.setY(1.0f);
			bitangent.setZ(0.0f);
		}
		else
		{
			det = 1.0f / det;

			tangent.setX((texEdge2.getY() * edge1.getX() - texEdge1.getY() * edge2.getX()) * det);
			tangent.setY((texEdge2.getY() * edge1.getY() - texEdge1.getY() * edge2.getY()) * det);
			tangent.setZ((texEdge2.getY() * edge1.getZ() - texEdge1.getY() * edge2.getZ()) * det);
			w = 0.0f;

			bitangent.setX((-texEdge2.getX() * edge1.getX() + texEdge1.getX() * edge2.getX()) * det);
			bitangent.setY((-texEdge2.getX() * edge1.getY() + texEdge1.getX() * edge2.getY()) * det);
			bitangent.setZ((-texEdge2.getX() * edge1.getZ() + texEdge1.getX() * edge2.getZ()) * det);

			tangent=tangent.normal();
			bitangent=bitangent.normal();
		}

		vector3f n(v1->nx, v1->ny, v1->nz);
		vector3f t(tangent.getX(), tangent.getY(), tangent.getZ());
		vector3f b;

		b = n.cross(t);
		w = (b.dot(bitangent) < 0.0f) ? -1.0f : 1.0f;

		v1->tx = tangent.getX(); v1->ty = tangent.getY(); v1->tz = tangent.getZ(); v1->tw = w;
		v2->tx = tangent.getX(); v2->ty = tangent.getY(); v2->tz = tangent.getZ(); v2->tw = w;
		v3->tx = tangent.getX(); v3->ty = tangent.getY(); v3->tz = tangent.getZ(); v3->tw = w;
	}

	inline int getVertexSize(FVF fvf) {
		switch (fvf) {
		case fvf_common: return sizeof(vertexCom);
		case fvf_2DT: return sizeof(vertex2D);
		case fvf_NT: return sizeof(vertexNT);
		default:throw("Vertex.h::getVertexSize --- fvf not exist");
		}
	}
}