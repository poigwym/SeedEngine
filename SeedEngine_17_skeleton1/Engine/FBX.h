
#include<fbxsdk.h>
#include"prequest.h"
#include"math3d.h"
#include"SubMesh.h"

#pragma comment(lib, "libfbxsdk.lib")

namespace Seed
{
#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(sdk->GetIOSettings()))
#endif


	class FBX
	{
	private:
		static int m_nTrangle;

	public:
		static void parse(const string& file, SubMesh *mesh);
		static void processNode(FbxNode* node, SubMesh *mesh);
		static void processMesh(FbxNode* node, SubMesh *mesh);
		static void readNormal(FbxMesh *mesh, int ctrlPointIndex, int vertexCounter, vector3f &normal);
		static void readUV(FbxMesh *mesh, int ctrlPointIndex, int UVindex,  vector2f &normal);
		static void readMaterial(FbxNode* node, SubMesh *mesh);

	};


	void InitializeSdkObjects(FbxManager*& sdk, FbxScene*& pScene);
	void DestroySdkObjects(FbxManager* sdk, bool pExitStatus);
	bool loadScene(FbxManager* sdk, FbxDocument* pScene, const char* pFilename);
}
