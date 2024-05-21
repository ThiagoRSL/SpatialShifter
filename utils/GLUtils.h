#ifndef GLUTILS_H
#define GLUTILS_H
#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "glm/glm.hpp"
#include <string>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using std::string;

namespace GLUtils
{
    int checkForOpenGLError(const char *, int);
    
    void dumpGLInfo(bool dumpExtensions = false);
    
	void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length, const GLchar * msg, const void * param );
}

#endif // GLUTILS_H
