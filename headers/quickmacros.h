#pragma once
#ifndef QUICKRENDER_MACROS
#define QUICKRENDER_MACROS


#include <quickrender.h>
//this exists purely because I'm too lazy to type out certain shit

#define zerovec vec3(0,0,0)
#define cstring const char*

static bool glPrintErrors(const char* function, const char* file, int line)
{
	using namespace std;
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		while (err != GL_NO_ERROR)
		{
			const char* strerror = "";
			switch (err)
			{
			case 1280:
				strerror = "Invalid enum";
				break;
			case 1281:
				strerror = "Invalid value";
				break;
			case 1282:
				strerror = "Invalid operation";
				break;


			case 1285:
				strerror = "OpenGL ran out of memory";
				break;
			}

			cout << strerror << " in function " << function << " in file " << file << " on line " << line << endl;
			return false;
		}
	}
	return true;
}


#define STOP_ON_FAILURE(x) if ((!x)) __debugbreak();

#define glCall(x)\
	x;\
	STOP_ON_FAILURE(glPrintErrors(#x, __FILE__, __LINE__));

#endif