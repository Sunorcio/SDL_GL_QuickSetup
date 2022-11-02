#ifndef GL_DEF
 #define GL_DEF 1
 #include <GL/glew.h>
#endif
#ifndef SDL2_DEF
 #define SDL2_DEF 1
 #include <SDL2/SDL.h>
#endif

//Configuration variables are defined in header file, edit them there

char catchSDLError(int functionreturn){
//feed a negative literal to just print SDL error, else load with a function call to detect and print error
	if(functionreturn<0){ SDL_Log("%s",SDL_GetError());}
	if(SDL_GetError() == 0){return -1;}
	return 0;
}
char catchGLError(){
//if no gl error return 0 else clear and print all errors
	GLenum error = glGetError();
	if (error == GL_NO_ERROR){ return 0; }

	do{SDL_Log("GLerror : %s", gluErrorString(error));}
	while(( error = glGetError() ));
	return -1;
}

void sdlglInit(SDL_Window** window, SDL_GLContext** context)
//initialises SDL window and GL context based on header definitions
{
	SDL_Init(SDL_INIT_VIDEO);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MINOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, PROFILE);

	*window 
	  =SDL_CreateWindow(0,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_OPENGL);
	*context = SDL_GL_CreateContext(*window);
	SDL_GL_SetSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(*window);
}


inline unsigned int shaderCompile(unsigned int type, const char* src)
{
	unsigned int shader = glCreateShader(type);
	glShaderSource(shader,1,&src,0);
	glCompileShader(shader);

	int compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		int l = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &l);

		char log[l];
		glGetShaderInfoLog(shader, l, &l, &log[0]);
		switch(type){
			case GL_VERTEX_SHADER:
				SDL_Log("couldnt compile vertex shader :");
				break;
			case GL_FRAGMENT_SHADER:
				SDL_Log("couldnt compile fragment shader :");
		}
		SDL_Log("%s",log);
		return 0;
	}

	return shader;
}

unsigned int shaderCreate(const char* vsrc, const char* fsrc)
{
	unsigned int sp = glCreateProgram();
	unsigned int vs;
	unsigned int fs;

	if(vsrc){
	vs = shaderCompile(GL_VERTEX_SHADER, vsrc);
	glAttachShader(sp,vs);
	}

	if(fsrc){
	fs = shaderCompile(GL_FRAGMENT_SHADER, fsrc);
	glAttachShader(sp,fs);
	}

	glLinkProgram(sp);
	glValidateProgram(sp);

	glDetachShader(sp,vs);
	glDeleteShader(vs);
	glDetachShader(sp,fs);
	glDeleteShader(fs);
	return sp;
}

