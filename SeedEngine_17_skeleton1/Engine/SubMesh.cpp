#include"SubMesh.h"



namespace Seed
{

	SubMesh::SubMesh()
	{
		mMaterial = 0;
		mGeometry = 0;
	}
	SubMesh::~SubMesh()
	{
		DELETE(mMaterial);
		DELETE(mGeometry);
	}

	SubMesh*  SubMesh::setMaterial(Material *mtl)
	{
		DELETE(this->mMaterial);
		this->mMaterial = mtl;

		return this;
	}


	SubMesh*  SubMesh::setGeometry(Geometry *Geometry)
	{
		DELETE(this->mGeometry);

		this->mGeometry = Geometry;
		return this;

	}

}
