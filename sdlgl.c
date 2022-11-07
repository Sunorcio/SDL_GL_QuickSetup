#ifndef GL_DEF
 #define GL_DEF 1
 #include <GL/glew.h>
#endif
#ifndef SDL2_DEF
 #define SDL2_DEF 1
 #include <SDL2/SDL.h>
#endif
#ifndef USR_DEF
 #include <string.h>
 #include <stdint.h>
#endif

//Configuration variables are defined in header file, edit them there, ignore compiler warnings

char catchSDLError(int functionreturn){
	if(functionreturn<0){ SDL_Log("%s",SDL_GetError());}
	if(SDL_GetError() == 0){return -1;}
	return 0;
}
char catchGLError(){
	GLenum error = glGetError();
	if (error == GL_NO_ERROR){ return 0; }

	do{SDL_Log("GLerror : %s", gluErrorString(error));}
	while(( error = glGetError() ));
	return -1;
}

void sdlglInit(SDL_Window** window, SDL_GLContext** context)
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, PROFILE);

	*window 
	  =SDL_CreateWindow(0,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
	*context = SDL_GL_CreateContext(*window);
	glewExperimental = GL_TRUE;
	glewInit();

	SDL_GL_SetSwapInterval(VSYNC);

	#if(BLENDING)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	#endif

	#if(DEPTH)
	glEnable(GL_DEPTH_TEST);
	#endif

	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(*window);
}



unsigned int sdlglShaderLoadnCompile(unsigned int shaderType, const char* srcfilepath)
{
	FILE* f = fopen(srcfilepath, "a+");
	fseek(f, 0, SEEK_END);
	int l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty");fclose(f);return 0;}
	if(l>SHADERMAXCHARLENGTH){SDL_Log("Shader exceeds character limit(defined in sdlgl.h)");fclose(f);return 0;}
	char shaderstring[l+1];
	memset(shaderstring, 0, l+1);
	fseek(f, 0, SEEK_SET);
	fread(shaderstring, 1, l, f);
	fclose(f);

	const char* shadersrc = shaderstring;
	unsigned int shaderObject = glCreateShader(shaderType);
	glShaderSource(shaderObject,1,&shadersrc,0);
	glCompileShader(shaderObject);

#ifdef DBG
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &l);
	if(!l)
	{
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &l);
		char log[l];
		glGetShaderInfoLog(shaderObject, l, &l, &log[0]);
		SDL_Log("Compilation failed - %s",log);
		return 0;
	}
#endif
	return shaderObject;
}

char* sdlglShaderLoadSource(const char* srcfilepath)
// Loads sourcefile into heap for hotreloading
{
	char* srcpointer = calloc(SHADERMAXCHARLENGTH, sizeof(char));
	FILE* f = fopen(srcfilepath, "a+");
	fseek(f, 0, SEEK_END);
	int l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty");fclose(f);return 0;}
	if(l>SHADERMAXCHARLENGTH){SDL_Log("Shader exceeds character limit(defined in sdlgl.h)");fclose(f);return 0;}
	memset(srcpointer, 0, l+1);
	fseek(f, 0, SEEK_SET);
	fread(srcpointer, 1, l, f);
	fclose(f);
	return srcpointer;
}

uint8_t sdlglShaderChecknReload(char** src,const char* srcfilepath,unsigned int* shaderObject, GLuint shaderType)
// If sourcefile is changed it compiles and returns 1, 0 otherwise
{
	FILE* f = fopen(srcfilepath, "a+");
	fseek(f, 0, SEEK_END);
	int l = ftell(f);
	if(!l){SDL_Log("Shader file missing or empty");fclose(f);return 0;}
	if(l>SHADERMAXCHARLENGTH){SDL_Log("Shader exceeds character limit(defined in sdlgl.h)");fclose(f);return 0;}
	char shaderstring[l+1];
	memset(shaderstring, 0, l+1);
	fseek(f, 0, SEEK_SET);
	fread(shaderstring, 1, l+1, f);
	fclose(f);
	
	if(strcmp(*src, shaderstring))
	{
		memset(*src, 0, SHADERMAXCHARLENGTH);
		memcpy(*src, shaderstring, l+1);
		glDeleteShader(*shaderObject);
		*shaderObject = glCreateShader(shaderType);
		glShaderSource(*shaderObject,1,(const char**)src,0);
		glCompileShader(*shaderObject);

		#ifdef DBG
		glGetShaderiv(*shaderObject, GL_COMPILE_STATUS, &l);
		if(!l)
		{
			glGetShaderiv(*shaderObject, GL_INFO_LOG_LENGTH, &l);
			char log[l];
			glGetShaderInfoLog(*shaderObject, l, &l, &log[0]);
			SDL_Log("Compilation failed - %s",log);
			return 0;
		}
		#endif

		return 1;
	}
	return 0;
}


