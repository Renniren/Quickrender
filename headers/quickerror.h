#pragma once
#ifndef QUICKERROR
#define QUICKERROR

#include <quickrender.h>

#define pragmatism __debugbreak(); 
#define panic __debugbreak();

void PrintErrors()
{
	using namespace std;
	GLenum err = glGetError();
	
	if (err == 0) return;

	switch (err)
	{
	case 1280:
		cout << "Error: 1280 - Invalid enum. Perhaps you used an enum where it shouldn't be?" << endl;
		break;

	case 1281:
		cout << "Error: 1281 - Invalid value. Perhaps you passed the wrong type of data somewhere?" << endl;
		break;

	case 1282:
		cout << "Error: 1282 - Invalid operation." << endl;
		break;

	case 1283:
		cout << "Error: 1283 - stack overflow." << endl;
		break;

	case 1284:
		cout << "Error: 1284 - stack... underflow? How the fuck did you even do that?" << endl;
		break;

	case 1285:
		cout << "Error: 1285 - OpenGL ran out of memory." << endl;
		break;

	case 1286:
		cout << "Error: 1286 - Invalid framebuffer read/write." << endl;
		break;
	}

}


#endif