
//Include GLEW - always first 
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "GLUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <filesystem>

//Include the standard C++ headers 
#include "manager/RenderManager.h"
#include "manager/UpdateManager.h"
#include "manager/CollisionManager.h"
#include "manager/CameraManager.h"
#include "manager/InputManager.h"
#include "manager/StageManager.h"
//#include "manager/InterfaceManager.h"
#include "manager/TextManager.h"
#include "builders/ShipBuilder.h"
#include "builders/InterfaceBuilder.h"
#include "entities/Entity.h"
#include "entities/Ship.h"

// Default window size.
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

// Inicialize a variável estática fora da classe

using namespace std;
InterfaceElement* Background;

Ship* PlayerShip;
Ship* tempEnemyShip;
GLFWwindow* window;

void mouseCallback(GLFWwindow* window, double xpos, double ypos);

void TimeUpdate(double deltaTime)
{
	// Atualiza as posições das entidades
	UpdateManager::GetInstance()->UpdateElements(deltaTime);
	// Atualiza a posição da camera, as vezes ela tá lock então precisa ser executada após
	// a atualização das entidades.
	//	CameraManager::GetInstance()->Update();
	// Verifica as colisões, parte da física talvez deva ter um framerate separado.
	CollisionManager::GetInstance()->VerifyCollisions();
	// Verifica interações com mouse e teclado
	InputManager::GetInstance()->Update(deltaTime);
	// Atualiza o estado do mundo
	StageManager::GetInstance()->UpdateStage(deltaTime);
}

void Render()
{
	//Background->Render();
	RenderManager::GetInstance()->RenderElements();
	InterfaceManager::GetInstance()->RenderInterface();
	InputManager::GetInstance()->RenderCursor();

	TextElement* telem = new TextElement();
	telem->Position = vec2(100.0f, 100.0f);
	telem->Text = "Vascao da Gama";
	telem->TextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	telem->Scale = 1.0f;
	telem->LineSpacing = 1.0f;
	telem->LineWidthMax = 200.0f;
	telem->MarginLeft = 0.0f;
	telem->MarginTop = 0.0f;

	TextManager::GetInstance()->RenderText(telem);

}

void mainLoop()
{
	double thisTime;
	double deltaTime;
	int lastFrame = 0;
	double lastTime = glfwGetTime();
	do{
		// Check for OpenGL errors
		GLUtils::checkForOpenGLError(__FILE__, __LINE__);


		thisTime = glfwGetTime();
		UpdateManager::GetInstance()->CheckTime(thisTime);
		
		if(lastFrame >= UpdateManager::GetInstance()->GetLastFrame()) continue;

		lastFrame = UpdateManager::GetInstance()->GetLastFrame();


		//Limpa tudo
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);


		// Atualiza os temporizadores e chama o update.
		TimeUpdate(thisTime - lastTime);
		// printf("\n\n FRAME: %i | LASTTIME: %lf | THISTIME: %lf | DELTATIME: %lf", lastFrame, lastTime, thisTime, thisTime - lastTime);
		lastTime = thisTime;

		// Renderiza tudo.
		Render();

		glfwSwapBuffers(window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...  
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));
}

// Define an error callback  
void errorCallback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

// Mouse callback
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager::GetInstance()->UpdateMousePosition(xpos, ypos);
}

void mouseScrollCallback(GLFWwindow* window, double xpos, double ypos)
{
	InputManager::GetInstance()->UpdateMouseScroll(xpos, ypos);
}


// Key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager::GetInstance()->KeyboardInput(key, scancode, action, mods);
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, source = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, source, message);
}

// Initialize GLFW  
void initGLFW()
{
	cout << "Initializing GLFW..." << endl;
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	
	if(GlobalVars::FULL_SCREEN)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		window = glfwCreateWindow(mode->width, mode->height, "GLSL4.0 - Tessellation Displacement Mapping", monitor, NULL);
	}
	else
	{
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "GLSL4.0 - Tessellation Displacement Mapping", NULL, NULL);
	}

	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		system("pause");
		exit(EXIT_FAILURE);
	}
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initCallbacks()
{
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, mouseScrollCallback);
	glfwSetErrorCallback(errorCallback);
}

void initGLEW()
{
	cout << "Initializing GLEW..." << endl;
	// Initialize GLEW
	glewExperimental = GL_TRUE; //ensures that all extensions with valid entry points will be exposed.
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		system("pause");
		exit(EXIT_FAILURE);
	}
	GLUtils::checkForOpenGLError(__FILE__, __LINE__); // Will throw error. Just ignore, glew bug.
	GLUtils::dumpGLInfo();

	//errors
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
}
void initializeGL()
{
	cout << "Initializing GL..." << endl;
	glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLUtils::checkForOpenGLError(__FILE__, __LINE__);
}
// Close OpenGL window and terminate GLFW  
void closeApplication()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

int main(int argc, char* argv[])
{
	(void)argc;
	std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());
	initGLFW();
	initCallbacks();
	initGLEW();
	initializeGL();
	InputManager::GetInstance()->SetWindow(window);
	CameraManager::GetInstance()->InitCamera(window);
	//CameraManager::GetInstance()->UpdateCameraSettings();
	//Se isso for carregado depois dos monstros o mouse para de renderizar ???

	tempEnemyShip = new Ship(ShipTypeId::STANDARD_SHIP_ID);
	tempEnemyShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	tempEnemyShip->SetLinearSpeed(vec3(0.0f, 0.0f, 0.0f));
	tempEnemyShip->SetShipColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	tempEnemyShip->SetMass(3500);
	tempEnemyShip->SetHitPoints(100.0f);
	tempEnemyShip->SetThrustIntensityMax(300000.0f);
	tempEnemyShip->SetFaction(Faction::PLAYER);
	tempEnemyShip->Init();

	PlayerShip = new Ship(ShipTypeId::STANDARD_SHIP_ID);
	PlayerShip->SetPosition(vec3(0.0f, 0.0f, 0.0f));
	PlayerShip->SetLinearSpeed(vec3(0.0f, 0.0f, 0.0f));
	PlayerShip->SetShipColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	PlayerShip->SetMass(3500);
	PlayerShip->SetHitPoints(100.0f);
	PlayerShip->SetThrustIntensityMax(300000.0f);
	PlayerShip->SetFaction(Faction::PLAYER);
	PlayerShip->Init();

	StageManager::GetInstance()->SetPlayer(PlayerShip);
	InputManager::GetInstance()->SetDebugMouse(tempEnemyShip);
	InputManager::GetInstance()->SetPlayerShip(PlayerShip);
	CameraManager::GetInstance()->SetTargetPoint(PlayerShip->GetPositionRef());

	//Background = new InterfaceElement();
	if (GlobalVars::FULL_SCREEN)
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		InterfaceManager::GetInstance()->SetInterfaceProjection(glm::ortho(0.0f, (float)mode->width, 0.0f, (float)mode->height));
		//Background->SetPosition(vec2(0.0f, mode->height));
		//Background->SetHeight(mode->height);
		//Background->SetWidth(mode->width);
	}
	else
	{
		InterfaceManager::GetInstance()->SetInterfaceProjection(glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT));
		//Background->SetPosition(vec2(0.0f, WINDOW_HEIGHT));
		//Background->SetHeight(WINDOW_HEIGHT);
		//Background->SetWidth(WINDOW_WIDTH);
	}
	//Background->SetTexture(GlobalPaths::BACKGROUND_IMAGE_PATH.c_str());


	InterfaceBuilder::BuildQuickBar();
	InterfaceBuilder::BuildInventory();

	mainLoop();

	exit(EXIT_SUCCESS);
}
