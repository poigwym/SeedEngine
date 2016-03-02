#include"CoreHeaders.h"
#include"Test.h"
#include"MeshOBJ.h"
#include"FBX.h"
#include"d3d9RenderingEngine.h"
using namespace Seed;

class EGame : public Game
{
public:
	Camera *camera;
	GameObject *obj1, *obj2;
	DirectionLight *light;
	Material *dfmtl;

	GameObject *ts;
	SubMesh *mesh;

	EGame() { }
	~EGame() {
		clear();
	}

	void init() {
		engine->getWindow()->setTitle("game");
		engine->getWindow()->setBkcolor(Color(180, 110, 50));

		camera = new Camera(vector3f(1, 2, -10), Quaternion(vector3f(0,1,0),0), pi / 4, engine->getWindow()->getWidth(), engine->getWindow()->getHeight(), 1.0f, 3000.0f);
		engine->getSceneManager()->setMainCamera(camera);

		Material *mtl = MaterialManager::getInstance().createMaterial("test.material");


		dfmtl = MaterialManager::getInstance().createMaterial("default.material");


		string objfile = "mesh/box.obj";
		string fbxfile = "mesh/ca.FBX";


		mesh = new SubMesh;
		//MeshOBJ::parse(file, mesh);

		FBX::parse(fbxfile, mesh);

		//OBJmesh objmesh, objsphere;
		/*

		obj1 = new GameObject;
		obj2 = new GameObject;

		light = new DirectionLight(vector3f(0, 10, 10), vector3f(0, -1, -1), vector3f(1, 1, 1), 1, true);


		obj1->setPos(vector3f(1, 1.5, 0))
			->setRot(Quaternion(vector3f(0, 1, 0), pi / 6))
			->setScale(vector3f(1, 1, 1))
			//->addMesh(mesh)
			->addMaterial(mtl);


		obj2
			-> setPos(vector3f(1, 1, -4))
			->setScale(vector3f(1, 1, 1))
			//->addMesh(mesh)
			->addMaterial(mtl);*/

			ts = new AA;
		ts
			->setPos(vector3f(0, 5, 0))
			//->addMaterial(dfmtl)
			->setPos(vector3f(0, 0, 0))
			->setRot(Quaternion(vector3f(1,0,0), pi/2))
			->update()
			;

		engine->getSceneManager()->getRoot()->addObject(ts);
		//engine->getSceneManager()->getRoot()->addObject(obj1);
		//engine->getSceneManager()->getRoot()->addObject(obj2);

	}

	void update() {
		Log(" update scene begin ");
		//obj1->setRot(obj1->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));
		//obj2->setRot(obj2->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));
		Log(" update scene end");
	}

	void render()
	{
		Log("begin render scene");


		dfmtl->applyMaterial(ts, camera, 1, 1);
		ts->_render(camera);
		RenderingEngine::getInstance().renderGeometry(mesh->getGeometry());

		Log("end render scene");

	}

	void clear() { }
};


int main(WindowID id)
{
	Engine *engine = new Engine(id, new  EGame);

	try
	{
		engine->run();
	}
	catch (char *s) {
		MessageBox(0, s, 0, 0);
		exit(1);
	}
	delete engine;
	return 0;
}

