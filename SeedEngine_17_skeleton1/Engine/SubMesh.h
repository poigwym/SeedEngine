#pragma once
#include"prequest.h"


namespace Seed
{

	class Material;
	class Geometry;

	class SubMesh
	{
	protected:
		Material *mMaterial;
		Geometry *mGeometry;
	public:
		SubMesh();
		virtual ~SubMesh();

		SubMesh* setMaterial(Material *mtl);
		inline Material *getMaterial() { return mMaterial; }

		SubMesh* setGeometry(Geometry *Geometry);
		inline Geometry* getGeometry() { return mGeometry; }
	};

}