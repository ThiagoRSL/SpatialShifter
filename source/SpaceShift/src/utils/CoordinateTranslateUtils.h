#pragma once

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "GLUtils.h"


class CoordinateTranslateUtils
{
	public:static glm::vec3 ScreenToWorld(double screenX, double screenY, int windowWidth, int windowHeight, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        printf("\nScreen (%f, %f)", screenX, screenY);

        screenX = (screenX + windowWidth / 2);
        screenY = (-screenY + windowHeight / 2);
        printf("After Transform (%f, %f)", screenX, screenY);

        glm::vec4 viewport = glm::vec4(0.0f, 0.0f, windowWidth, windowHeight);


        // Desprojetar para obter o ponto inicial da linha
        glm::vec3 wincoord = glm::vec3(screenX, screenY, 0.0f);
        glm::vec3 lineStart = glm::unProject(wincoord, viewMatrix, projectionMatrix, viewport);

        // Desprojetar para obter o ponto final da linha
        wincoord = glm::vec3(screenX, screenY, 1.0f);
        glm::vec3 lineEnd = glm::unProject(wincoord, viewMatrix, projectionMatrix, viewport);

        // Calcular a interseção da linha com o plano z = 1
        glm::vec3 lineDir = glm::normalize(lineEnd - lineStart);
        float t;
        if (lineDir.z != 0.0)
            t = (1.0f - lineStart.z) / lineDir.z;
        else
            t = 0;
        glm::vec3 objcoord = lineStart + t * lineDir;

        return objcoord;
    }
};