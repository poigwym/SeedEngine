

#pragma once


#include<iostream>
#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<algorithm>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include<utility>
#include<fstream>
#include<sstream>
#include<cassert>
#include<iomanip>
#include<windows.h>
#include<tchar.h>

#include<Windows.h>
#include<tchar.h>
#include<ctime>

#include<d3dx9.h>
#include<d3d9.h>


#include"util.h"

using std::vector;
using std::string;
using std::set;
using std::map;
using std::queue;
using std::stack;
using std::iostream;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::ios;



#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "Winmm.lib")



namespace Seed{

	class Engine;
	class InputController;
	class Color;

	class Transform;
	class GameObject;
	class Billboard;
	class Window;
	class Camera;


	class VertexBuffer;


	class Material;
	class MaterialManager;

	class Texture;
	class TextureManager;


	class Shader;
	class RenderState;
	class RenderingEngine;
	class RenderTarget;

	class Light;
	class PointLight;
	class DirectionLight;
	class Shadow;
	class SphereMirror;
	class Node;
	class SceneManager;

	class Timer;

	// ----math
	class matrix4f;
	class Quaternion;
	class vector3f;


	// Geometry
	class Mesh;

	// -----------d3d9

	class d3d9RenderingEngine;
	class d3d9Texture;
	class d3d9TextureManager;
}