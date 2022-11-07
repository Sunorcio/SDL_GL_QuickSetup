#pragma once


//init variables, edit these
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define VSYNC 1 // 1^0
#define BLENDING 1 // 1^0
#define DEPTH 0 // 1^0  (3D rendering)
#define SHADERMAXCHARLENGTH 100000
#define MAJOR_VERSION 4
#define MINOR_VERSION 6
#define PROFILE SDL_GL_CONTEXT_PROFILE_CORE // CORE^COMPATIBILITY
//#define DBG

#include "sdlgl.c"

#define clearterminal() SDL_Log("\e[H\e[2J")

#ifdef DBG
 #define secureGL(x) catchGLError(); \
					 x; \
					 if(catchGLError()){SDL_Log("line : %d, function : %s",__LINE__,__FUNCTION__);}
 // Wrap gl functions with this then declare debug build with -DDBG to print gl errors and their locations in code
#else
 #define secureGL(x) x;
#endif



extern void sdlglInit(SDL_Window** window, SDL_GLContext** context);
//initialises SDL window and GL context based on above defines

extern char catchSDLError(int functionreturn);
//feed a negative literal to just print SDL error, else load with a function call to detect and print error

extern char catchGLError();
//if no gl error return 0 else clear and print all errors


extern unsigned int sdlglShaderLoadnCompile(unsigned int shaderType, const char* srcfilepath);
// load source from file into stack and compile into returned shaderobject

char* sdlglShaderLoadSource(const char* srcfilepath);
// Loads sourcefile into heap for hotreloading

uint8_t sdlglShaderChecknReload(char** src,const char* srcfilepath,unsigned int* shaderObject, GLuint shaderType);
// If sourcefile is changed it compiles and returns 1, 0 otherwise

