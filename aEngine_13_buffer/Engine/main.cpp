#include"OBJmesh.h"
#include"CoreHeaders.h"
using namespace NBengine;


class EGame : public Game
{
public:
	Camera *camera;
	GameObject *rootobj;
	GameObject *obj1, *obj2;
	GameObject *obj3;
	GameObject *sky;

	EGame() { }
	~EGame() {
		clear();
	}

	void init() {
		engine->getWindow()->setTitle("game");
		engine->getWindow()->setBkcolor(Color(180, 110, 50));

		camera = new Camera(vector3f(1, 2, -10), Quaternion(vector3f(0,1,0),0), pi / 4, engine->getWindow()->getWidth(), engine->getWindow()->getHeight(), 1.0f, 3000.0f);
		engine->getSceneManager()->setMainCamera(camera);

		Texture *tex = engine->getTextureGM()->create(1024, 1024, PXFMT_X8R8G8B8, 0, TEXF_LINEAR);
		//tex->load("picture/bricks2.jpg");
		tex->load("picture/test.png");
		Texture *normalMap = engine->getTextureGM()->create(1024, 1024, PXFMT_X8R8G8B8, 0, TEXF_LINEAR);
		normalMap->load("picture/bricks2_normal.jpg");


		//string file = "mesh/box.obj";
		OBJmesh objmesh, objsphere;
		Mesh *mesh = objmesh.parse("mesh/box.obj");
		Mesh *sphere = objsphere.parse("mesh/sphere.obj");

		rootobj = new GameObject;
		obj1 = new GameObject;
		obj2 = new GameObject;
		obj3 = new GameObject;
		sky = new GameObject;
		

		//// box rootobj
		rootobj->setScale(vector3f(50, 1, 50))
			->addMesh(mesh)
			->addChild(obj1)
			->addTexture(tex)
			->addNormalMap(normalMap);

		obj1->setPos(vector3f(1, 1.5, 0))
			->setRot(Quaternion(vector3f(0, 1, 0), pi / 6))
			->setScale(vector3f(1, 1, 1))
			->addMesh(mesh)
			->addChild(obj2)
			//->addChild(obj3)
			->addTexture(tex)
			->addNormalMap(normalMap);

		obj2->setPos(vector3f(1, 1, -4))
			->setScale(vector3f(1, 1, 1))
			->addMesh(mesh)
			->addTexture(tex)
			->addNormalMap(normalMap);

		obj3->setPos(vector3f(-10, 2, -1))
			->setScale(vector3f(1, 1, 1))
			->addMesh(sphere)
			->addTexture(tex)
			->addNormalMap(normalMap);

		sky->addMesh(sphere)->setScale(vector3f(10, 10, 10));
		engine->getSceneManager()->addSky(sky);
		
		engine->getSceneManager()->getRoot()->addObject(rootobj);
		engine->getSceneManager()->getRoot()->addObject(obj3);

	}

	void update() {
		obj1->setRot(obj1->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));
		//obj3->setRot(obj3->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));
		obj2->setRot(obj2->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));

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

