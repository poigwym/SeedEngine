#include"SubMesh.h"


namespace Seed
{
	class MeshOBJ
	{
		struct Vec3 { 
			float x, y, z;
			Vec3(){}
			Vec3(const Vec3& v) :x(v.x), y(v.y), z(v.z) {}
		};

		struct Vec2 {
			float x, y;
			Vec2(){}
			Vec2(const Vec2& v) :x(v.x), y(v.y){}
		};

	public:

		static void parse(const string &file, SubMesh *mesh);
		static void parse(const string &file, Geometry* geo);

	};

}