#include"math3d.h"

namespace NBengine
{
	vector3f vector3f::rotate(const Quaternion &q) const
	{
		Quaternion v((*this)[0], (*this)[1], (*this)[2], 0);
		v = q.conjugate()*v*q;		// clockwise
		return vector3f(v.getX(), v.getY(), v.getZ());
	}



}