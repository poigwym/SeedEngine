

#pragma once

namespace Seed{
	#define RELEASE(x) { if(x) (x)->Release(); x=0; }
	#define DELETE(x) { if(x) delete x; x=0;}	
	#define DELETEARRAY(x) { if(x) delete[] x; x=0; } 

#define MOUT(x)  MessageBox(0, x, 0, 0)
#define FUCK MessageBox(0, "fuck", 0, 0)


	typedef unsigned int  u32;
	typedef unsigned char uchar;
	typedef float real8;


	

}