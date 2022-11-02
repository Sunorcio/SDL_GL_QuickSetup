#pragma once

//init variables, edit these
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SHADER_MAXCHARLENGTH 100000
#define MAJOR_VERSION 4
#define MINOR_VERSION 6
#define PROFILE SDL_GL_CONTEXT_PROFILE_CORE // choose : CORE^COMPATIBILITY

#include "sdlgl.c"

#define clearterminal() SDL_Log("\e[H\e[2J")

#ifdef DBG
	//Wrap gl functions with this and use -DDBG to print gl errors and their locations in code
	#define secureGL(x) catchGLError(); \
						x; \
						if(catchGLError()){SDL_Log("line : %d, function : %s",__LINE__,__FUNCTION__);}
#else
	#define secureGL(x) x;
#endif


extern char catchSDLError(int functionreturn);
//feed a negative literal to just print SDL error, else load with a function call to detect and print error
extern char catchGLError();
//if no gl error return 0 else clear and print all errors

extern void sdlglInit(SDL_Window** window, SDL_GLContext** context);
//initialises SDL window and GL context based on init defines


extern char* shaderLoadfromfile();

extern unsigned int shaderCreate(const char* vsrc, const char* fsrc);

extern unsigned int shaderCompile(unsigned int type, const char* src);

