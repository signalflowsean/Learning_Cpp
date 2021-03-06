#pragma once

#include <GL/glew.h>
#include <signal.h>

using namespace std; 

#define LOG(x) cout << x << endl; 
#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
