
#include"camera.h"

namespace Seed
{
	vector3f Camera::yAxis = vector3f(0, 1, 0);


	void Camera::input()
	{

		float moveAmt = 0.15;
		if (Input->KeyDown(DIK_W))
			move(getForward(TS_local), moveAmt);
		if (Input->KeyDown(DIK_S))
			move(getForward(TS_local), -moveAmt);
		if (Input->KeyDown(DIK_D))
			move(getRight(TS_local), moveAmt);
		if (Input->KeyDown(DIK_A))
			move(getLeft(TS_local), moveAmt);

		float rotAmt = 0.01;

		rotateY(Input->GetMouseX()*rotAmt);
		rotateX(Input->GetMouseY()*rotAmt);

	}
}