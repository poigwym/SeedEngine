#include"OBJmesh.h"
#include"CoreHeaders.h"
using namespace Seed;


class EGame : public Game
{
public:
	Camera *camera;
	GameObject *obj1, *obj2;
	DirectionLight *light;
	GameObject *terrain;

	EGame() { }
	~EGame() {
		clear();
	}

	void init() {
		engine->getWindow()->setTitle("game");
		engine->getWindow()->setBkcolor(Color(180, 110, 50));

		camera = new Camera(vector3f(1, 2, -10), Quaternion(vector3f(0,1,0),0), pi / 4, engine->getWindow()->getWidth(), engine->getWindow()->getHeight(), 1.0f, 3000.0f);
		engine->getSceneManager()->setMainCamera(camera);


		//string file = "mesh/box.obj";
		OBJmesh objmesh, objsphere;
		Mesh *mesh = objmesh.parse("mesh/box.obj");
		Mesh *sphere = objsphere.parse("mesh/sphere.obj");

		obj1 = new GameObject;
		obj2 = new GameObject;
		
		Material *mtl = MaterialManager::getInstance().createMaterial("test.material");

		light = new DirectionLight(vector3f(0, 10, 10), vector3f(0, -1, -1), vector3f(1, 1, 1), 1, true);


		obj1->setPos(vector3f(1, 1.5, 0))
			->setRot(Quaternion(vector3f(0, 1, 0), pi / 6))
			->setScale(vector3f(1, 1, 1))
			->addMesh(mesh)
			->addMaterial(mtl);


		obj2->setPos(vector3f(1, 1, -4))
			->setScale(vector3f(1, 1, 1))
			->addMesh(mesh)
			->addMaterial(mtl);

		int w = 20, h = 20;
		int i, j;
		int wd = 1;
		Mesh *mesh1 = new Mesh;
		for (i = 0; i<w; ++i)
			for (j = 0; j < h; ++j)
			{
				int hh = rand() % 3;
				vertexNT v[] = {
				vertexNT(i*wd, 0, j*wd, 0, 1, 0, 0, 0),
				vertexNT((i + 1)*wd, 0, (j + 1)*wd, 0, 1, 0, 1, 1),
				vertexNT((i+1)*wd, 0, j*wd, 0, 1, 0, 0, 1),

				vertexNT(i*wd, 0, j*wd, 0, 1, 0, 0, 0),
				vertexNT(i*wd, 0, (j+1)*wd, 0, 1, 0, 1.0, 0),
				vertexNT((i + 1)*wd, 0, (j + 1)*wd, 0, 1, 0, 1, 1)
			};
			calTangent(v, v + 1, v + 2);
			calTangent(v + 3, v + 4, v + 5);
			mesh1->addFace(v[0], v[1], v[2]);
			mesh1->addFace(v[3], v[4], v[5]);
		}
		mesh1->build();

		terrain = new GameObject;
		terrain->addMesh(mesh1)
		->setPos(vector3f(-10,0,-10))
		->setScale(vector3f(1,1,1))
		->addMaterial(mtl) ;

		engine->getSceneManager()->getRoot()->addObject(terrain);
		engine->getSceneManager()->getRoot()->addObject(obj1);
		engine->getSceneManager()->getRoot()->addObject(obj2);

	}

	void update() {
		obj1->setRot(obj1->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));
		obj2->setRot(obj2->getRot(TS_local) * Quaternion(vector3f(0, 1, 0), 0.001));
	}

	void render()
	{
		vector<GameObject*> shadowq;
		shadowq.push_back(terrain);
		shadowq.push_back(obj1);
		shadowq.push_back(obj2);

		Window *w = RenderingEngine::getInstance().getWin32();
		float x = w->getMouseX();
		float y = w->getMouseY();
		float z = 1.0f;
		x = (x / w->getWidth() * 2 - 1) / camera->getProjection()[0][0];
		y = (-(y / w->getHeight() * 2 - 1)) / camera->getProjection()[1][1];
		vector3f pickpoint(x, y, z);
		vector3f ray = camera->getView().inverse().transformNormal(pickpoint);
		shadowq[1]->setPos(camera->getPos() + ray * 20)->updateTransform();

		////-------------xxxxoooo--------------------------------------
		u32 i = 0;
		Material *shadow = light->getShadow()->getMaterial();
		shadow->applyMaterial(0, 0, 1, 1);
		for (i = 0; i < shadowq.size(); ++i) {
			shadowq[i]->addLight(light);
			shadowq[i]->renderShadow(shadow);
		}

		//applyGaussBlur(512, 512, light->getShadow()->getShadowMap());

		// second pass 
		RenderingEngine::getInstancePtr()->getRenderTarget(0)->bindAsRenderTarget(0, false, true);
		for (i = 0; i < shadowq.size() ; ++i) {
			shadowq[i]->_render(camera);
		}
		////-----------xxxxxooooo--------------------------------
		//
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

