#include"FBX.h"
#include"Vertex.h"
#include"Material.h"
#include"Geometry.h"
#include"RenderingEngine.h"
#include"TextureManager.h"
#include"Bug.h"



namespace Seed
{
	void InitializeSdkObjects(FbxManager*& sdk, FbxScene*& pScene)
	{
		sdk = FbxManager::Create();
		FbxIOSettings* ios = FbxIOSettings::Create(sdk, IOSROOT);
		sdk->SetIOSettings(ios);
		FbxString lPath = FbxGetApplicationDirectory();
		sdk->LoadPluginsDirectory(lPath.Buffer());
		pScene = FbxScene::Create(sdk, "My Scene");
	}

	 void DestroySdkObjects(FbxManager* sdk, bool pExitStatus)
	{
		//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
		if (sdk) sdk->Destroy();
	}

	 bool loadScene(FbxManager* sdk, FbxDocument* pScene, const char* pFilename)
	{
		
		 bool lStatus;
		 // Create an importer.
		 FbxImporter* lImporter = FbxImporter::Create(sdk, "");

		 // Initialize the importer by providing a filename.
		 const bool lImportStatus = lImporter->Initialize(pFilename, -1, sdk->GetIOSettings());
		 // Import the scene.
		 lStatus = lImporter->Import(pScene);

		


		 // Destroy the importer.
		 lImporter->Destroy();

		 return lStatus;
	}










	int FBX::m_nTrangle=0;




	void FBX::parse(const string& file, SubMesh *mesh)
	{
		FbxManager *sdk = NULL;
		FbxScene *scene = NULL;

		bool res;
		FbxString path(file.c_str());
		// Prepare the FBX SDK.
		InitializeSdkObjects(sdk, scene);
		// Load the scene.
		res = loadScene(sdk, scene, path.Buffer());


		//FbxNode* node = scene->GetRootNode();
		processNode(scene->GetRootNode(), mesh);
		DestroySdkObjects(sdk, res);
	}

	void FBX::processNode(FbxNode* node, SubMesh *mesh)
	{

		if (node->GetNodeAttribute() &&
			node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			//Log << "find mesh!!" << endl;
			processMesh(node, mesh);
		}

		int c = node->GetChildCount();
		for (int i = 0; i < c; ++i)
			processNode(node->GetChild(i), mesh);
	}


	void FBX::processMesh(FbxNode* node, SubMesh *mesh)
	{
		FbxMesh* pMesh = (FbxMesh*)node->GetNodeAttribute();

		Material *mtl = mesh->getMaterial();
		if (!mtl)
		{
			mtl = MaterialManager::getInstance().createMaterial("default.material");
			mesh->setMaterial(mtl);
		}

		Geometry *geo = new Geometry;

		VertexDeclaration *decl = RenderingEngine::getInstance().createVertexDeclaration();

		// determine VertexDeclaration
		decl->addElementSemantic(VES_POSITION);
		if (pMesh->GetElementUVCount())
			decl->addElementSemantic(VES_TEXCOORD);
		if (pMesh->GetElementNormalCount())
			decl->addElementSemantic(VES_NORMAL);
		///......

		geo->setVertexDeclaration(decl);



		m_nTrangle = pMesh->GetPolygonCount();
		//Log << "point count " << pMesh->GetControlPointsCount() << endl;
		int vertexCounter = 0;				// 顶点在所在三角形的下标
		FbxVector4* lControlPoints = pMesh->GetControlPoints();
		for (unsigned int i = 0; i < m_nTrangle; ++i)
		{
			// three vertex of this triangle
			Vertex v[3];

			//  element's of three vertex
			vector3f pos[3];
			vector2f uv[3];
			vector3f normal[3];

			//vector3f tangent[3];
			//vector3f binormal[3];
			//Triangle currTriangle;
			//mTriangles.push_back(currTriangle);


			for (unsigned int j = 0; j < 3; ++j)
			{
				int ctrlPointIndex = pMesh->GetPolygonVertex(i, j);		// 顶点的下标
				FbxVector4 v4 = lControlPoints[ctrlPointIndex];

				v[j].addPosition(v4[0], v4[1], -v4[2]);  //lhs  reverse z

				if (pMesh->GetElementUVCount())  
				{
					readUV(pMesh, ctrlPointIndex, pMesh->GetTextureUVIndex(i, j), uv[j]);
					//v[j].addTexcoord(uv[j].getX(), uv[j].getY());
					//float uu = uv[j].getX(), vv = uv[j].getY();
					v[j].addTexcoord(uv[j].getX(), 1-uv[j].getY());
				}
				if (pMesh->GetElementNormalCount())
				{
					readNormal(pMesh, ctrlPointIndex, vertexCounter, normal[j]);
					//lhs  reverse z
					v[j].addNormal(normal[j].getX(), normal[j].getY(), -normal[j].getZ());
				}
				++vertexCounter;
			}
			
			geo->addTriangle(v[0], v[2], v[1]);
		}

		readMaterial(node, mesh);


		mesh->setGeometry(geo);
	}

	void FBX::readNormal(FbxMesh* pMesh, int inCtrlPointIndex, int inVertexCounter, vector3f& normal)
	{
		if (pMesh->GetElementNormalCount() < 1) {
			throw ("FBX::readNormal --- Invalid Normal Number");
		}

		FbxGeometryElementNormal* vertexNormal = pMesh->GetElementNormal(0);


		switch (vertexNormal->GetMappingMode())
		{
			case FbxGeometryElement::eByControlPoint: 
			{
				switch (vertexNormal->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						normal.setX(static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]));
						normal.setY(static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]));
						normal.setZ(static_cast<float>(vertexNormal->GetDirectArray().GetAt(inCtrlPointIndex).mData[2]));
					}
					break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int index = vertexNormal->GetIndexArray().GetAt(inCtrlPointIndex);
						normal.setX(static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]));
						normal.setY(static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]));
						normal.setZ(static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]));
					}
					break;

					default:
						throw ("FBX::readNormal ---Invalid Reference");
				}
			}
			break;

			case FbxGeometryElement::eByPolygonVertex:
			{
				switch (vertexNormal->GetReferenceMode())
				{
					case FbxGeometryElement::eDirect:
					{
						normal.setX(static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[0]));
						normal.setY(static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[1]));
						normal.setZ(static_cast<float>(vertexNormal->GetDirectArray().GetAt(inVertexCounter).mData[2]));
					}
					break;

					case FbxGeometryElement::eIndexToDirect:
					{
						int index = vertexNormal->GetIndexArray().GetAt(inVertexCounter);
						normal.setX(static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]));
						normal.setY(static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]));
						normal.setZ(static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]));
					}
					break;

					default:
						throw ("FBX::readNormal ---Invalid Reference");
				}
			}

			break;
		}
	}


	void FBX::readUV(FbxMesh *pMesh, int ctrlPointIndex, int UVindex, vector2f &tex)
	{
		FbxGeometryElementUV* UV = pMesh->GetElementUV(0);
		FbxVector2 v2;
		switch (UV->GetMappingMode())
		{
		default:
			break;
		case FbxGeometryElement::eByControlPoint:
			switch (UV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
					v2 = UV->GetDirectArray().GetAt(ctrlPointIndex);
				break;
			case FbxGeometryElement::eIndexToDirect:
			{
				int id = UV->GetIndexArray().GetAt(ctrlPointIndex);
				v2 = UV->GetDirectArray().GetAt(id);
			}
				break;
			default:
				break; // other reference modes not shown here!
			}
			break;

		case FbxGeometryElement::eByPolygonVertex:
		{
			switch (UV->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
			case FbxGeometryElement::eIndexToDirect:
				v2 = UV->GetDirectArray().GetAt(UVindex);
				break;
			default:
				break; 
			}
		}
			break;

		case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
		case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
		case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
			break;
		}

		tex.setX(v2[0]);
		tex.setY(v2[1]);
	}



	void FBX::readMaterial(FbxNode* node, SubMesh *mesh)
	{

		int nMtl = node->GetSrcObjectCount<FbxSurfaceMaterial>();
		//Log << "material num " << nMtl << endl;
		for (int index = 0; index<nMtl; index++)
		{
			FbxSurfaceMaterial *material = (FbxSurfaceMaterial*)node->GetSrcObject<FbxSurfaceMaterial>(index);
			if (material)
			{
				// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
				FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

				// Check if it's layeredtextures
				int layered_texture_count = prop.GetSrcObjectCount<FbxLayeredTexture>();
				if (layered_texture_count > 0)
				{
					for (int j = 0; j < layered_texture_count; j++)
					{
						FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
						int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();
						for (int k = 0; k<lcount; k++)
						{
							FbxTexture* texture = FbxCast<FbxTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
							// Then, you can get all the properties of the texture, include its name
							const char* texture_name = texture->GetName();
						}
					}
				}
				else
				{
					// Directly get textures
					int texture_count = prop.GetSrcObjectCount<FbxTexture>();
					for (int j = 0; j<texture_count; j++)
					{
						 FbxTexture* texture = FbxCast<FbxTexture>(prop.GetSrcObject<FbxTexture>(j));
						// Then, you can get all the properties of the texture, include its name
						const string texture_name = texture->GetName();
						FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(texture);
						Log("model texture ");
						Log(lFileTexture->GetFileName());
						const string &file = lFileTexture->GetFileName();

						unsigned context;
						for(context = file.size(); context>=0;--context) 
						if (file[context] == '/' || file[context] == '\\') break;
						if (context >= 0)
						{
							Log(string("mesh/") +
								file.substr(context + 1, file.length() - context - 1));
							mesh->getMaterial()->setDiffuseMap(
								TextureManager::getInstance().
								/*create(
								512, 512, PF_A8R8G8B8, 0, string("mesh/") +
								file.substr(context+1, file.length() - context - 1))
								);*/

								createFromFile(
								string("mesh/") +
								file.substr(context + 1, file.length() - context - 1))
								);
						}
					}
				}
			}
		}
	}

}