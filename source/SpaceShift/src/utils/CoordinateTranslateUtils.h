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
	public:static glm::vec3 ScreenToWorld(double z_target, double screenX, double screenY, int windowWidth, int windowHeight, glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
    {
        printf("\nScreen (%f, %f)", screenX, screenY);
        
        float x = (screenX + windowWidth)/2;
        float y = (-screenY + windowHeight)/2;
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
    // Função para converter as coordenadas da tela para o mundo 3D
    public:static glm::vec3 ScreenToWorld(int screenX, int screenY, int windowWidth, int windowHeight, glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float z_target)
    {
        // Normalizar as coordenadas de tela para o intervalo [-1, 1]
        float x = (screenX + windowWidth)/2;
        float y = (-screenY + windowHeight)/2;  // Atenção à inversão de y

        // Definir a coordenada z como 0 (para plano próximo) e 1 (para plano distante)
        glm::vec3 nearPoint(x, y, 0.0f);
        glm::vec3 farPoint(x, y, 1.0f);

        // Definir o viewport
        glm::vec4 viewport(0.0f, 0.0f, windowWidth, windowHeight);

        // Desprojetar as coordenadas de tela para 3D (com a coordenada z = 0.0f para near e z = 1.0f para far)
        glm::vec3 worldNear = glm::unProject(nearPoint, viewMatrix, projectionMatrix, viewport);
        glm::vec3 worldFar = glm::unProject(farPoint, viewMatrix, projectionMatrix, viewport);

        // Calcular a direção da linha (do ponto próximo até o ponto distante)
        glm::vec3 direction = glm::normalize(worldFar - worldNear);

        // Calcular o parâmetro t para a interseção com o plano z = z_target
        float t = (z_target - worldNear.z) / direction.z;

        // Calcular o ponto de interseção com o plano z = z_target
        glm::vec3 intersection = worldNear + t * direction;

        return intersection;
    }
};