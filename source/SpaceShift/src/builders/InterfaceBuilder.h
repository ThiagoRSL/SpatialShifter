#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>
#include <vector>

#include "TextureManager.h"
#include "GLUtils.h"
#include "../manager/InterfaceManager.h"

class InterfaceBuilder
{	

private:	
	InterfaceBuilder();
	
public:
	static void BuildInventory();
	static void BuildQuickBar();
};