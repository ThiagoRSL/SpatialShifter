
#include "InputManager.h"
#include "InterfaceManager.h"

//add to glfwGetKey that gets the pressed key only once (not several times)
char keyOnce[GLFW_KEY_LAST + 1];
#define glfwGetKeyOnce(WINDOW, KEY)             \
    (glfwGetKey(WINDOW, KEY) ?              \
     (keyOnce[KEY] ? false : (keyOnce[KEY] = true)) :   \
     (keyOnce[KEY] = false))

bool lastPressed = false;
bool lastPressed2 = false;

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 projectionMatrix;
glm::mat4 modelViewMatrix;

int windowWidth, windowHeight;
float mouseLastX = 0.0f, mouseLastY = 0.0f;
bool unintializedMouse = true;

using namespace std;


InputManager* InputManager::instance = nullptr;


InputManager::InputManager()
{

}

InputManager::InputManager(GLFWwindow* window, int size)
{
	size = size;
}

void InputManager::Init()
{
	Model = CursorModel();
	CameraInstance = CameraManager::GetInstance();
	Model.Init();
}

void InputManager::Update(double deltaTime)
{
	this->ProcessMouse(deltaTime);
	this->VerifyMouseCollision();
}



void InputManager::KeyboardInput(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	// Rotation on Z axis
	switch (key)
	{
	case (GLFW_KEY_A):
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationZ(-1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationZ(0.0f);
		break;
	case (GLFW_KEY_D):
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationZ(1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationZ(0.0f);
		break;
	case (GLFW_KEY_Q):
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationX(-1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationX(0.0f);
		break;
	case (GLFW_KEY_E):
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationX(1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationX(0.0f);
		break;
	case (GLFW_KEY_I):
		if (action == GLFW_PRESS)
		{
			InterfaceManager::GetInstance()->ToggleRender(InterfaceElementIndex::INVENTORY);
		}
		//else if (action == GLFW_RELEASE)				
		break;
	case (GLFW_KEY_L):
		if (action == GLFW_PRESS)
		{
			StageManager::GetInstance()->SetKillCount(150);
		}
		//else if (action == GLFW_RELEASE)				
		break;
	case (GLFW_KEY_W):
		if (action == GLFW_PRESS)
			PlayerShip->SetThrustMode(1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->SetThrustMode(0.0f);
		break;
	case (GLFW_KEY_S):
		if (action == GLFW_PRESS)
			PlayerShip->SetThrustMode(-1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->SetThrustMode(0.0f);
		break;
	case (GLFW_KEY_LEFT_SHIFT):
		if (action == GLFW_PRESS)
			LeftShiftPressed = true;
		else if (action == GLFW_RELEASE)
			LeftShiftPressed = false;
		break;
	case (GLFW_KEY_LEFT_CONTROL):
		if (action == GLFW_PRESS)
			LeftCtrPressed = true;
		else if (action == GLFW_RELEASE)
			LeftCtrPressed = false;
		break;
	case (GLFW_KEY_LEFT_ALT):
		if (action == GLFW_PRESS)
		{
			AutonomyShipModule* PlayerAutonomy = PlayerShip->GetAutonomy();
			if (PlayerAutonomy != NULL)
			{
				PlayerShip->SetAutonomous(true);
				PlayerAutonomy->SetBehaviour(AutonomyBehavior::STABILIZE);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			AutonomyShipModule* PlayerAutonomy = PlayerShip->GetAutonomy();
			if (PlayerAutonomy != NULL)
			{
				PlayerShip->SetAutonomous(false);
				PlayerAutonomy->SetBehaviour(AutonomyBehavior::NONE);
			}
		}
		break;
	}
}
void InputManager::ProcessMouse(double deltaTime)
{



	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (LeftCtrPressed)
		{
			vec3 shotTarget = CameraManager::GetInstance()->WorldPivot() +
				CoordinateTranslateUtils::ScreenToWorld(mousePosition.x, mousePosition.y, windowSize.x, windowSize.y,
					CameraInstance->ViewMatrix(), CameraInstance->ProjectionMatrix(), PlayerShip->GetPosition().z);

			AutonomyShipModule *Autonomy = DebugEnemy->GetAutonomy();
			if (Autonomy != NULL)
			{
				SeekTask* t = new SeekTask(shotTarget);
				Autonomy->AddTaskEnd(t);
			}

			return;
		}

		if (!lastPressed)
		{
			lastPressed = true;
			if (InterfaceManager::GetInstance()->GetMouseHoverElement() != NULL)
			{
				InterfaceManager::GetInstance()->GetMouseHoverElement()->Interact(InterfaceInteraction::MOUSE_CLICK_LEFT);
			}
			else
			{
				//Aqui t� errado, as coordenadas n�o representam.
				vec3 shotTarget = CameraManager::GetInstance()->WorldPivot() +
					CoordinateTranslateUtils::ScreenToWorld(mousePosition.x, mousePosition.y, windowSize.x, windowSize.y,
						CameraInstance->ViewMatrix(), CameraInstance->ProjectionMatrix(), PlayerShip->GetPosition().z);

				PlayerShip->ShotAt(shotTarget);
				//DebugEnemy->SetPosition(shotTarget);
			}
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		if (lastPressed)
		{
			lastPressed = false;
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		if (!lastPressed2)
		{
			lastPressed2 = true;

			if (InterfaceManager::GetInstance()->GetMouseHoverElement() != NULL)
			{
				InterfaceManager::GetInstance()->GetMouseHoverElement()->Interact(InterfaceInteraction::MOUSE_CLICK_RIGHT);
			}
			else
			{
				//Aqui t� errado, as coordenadas n�o representam.
				vec3 shotTarget = CameraManager::GetInstance()->WorldPivot() +
					CoordinateTranslateUtils::ScreenToWorld(mousePosition.x, mousePosition.y, windowSize.x, windowSize.y,
						CameraInstance->ViewMatrix(), CameraInstance->ProjectionMatrix(), PlayerShip->GetPosition().z);

				shotTarget.z = PlayerShip->GetPosition().z;

				PlayerShip->UseSkill(shotTarget);
			}
		}
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		if (lastPressed2)
		{
			lastPressed2 = false;
		}
	}

}


void InputManager::UpdateMouseScroll(float xpos, float ypos)
{
	float delta = -(ypos * 2.0f);
	CameraManager::GetInstance()->ChangeSize(delta);
}

void InputManager::UpdateMousePosition(float xpos, float ypos)
{
	xpos = MathUtils::Clip(xpos, -windowSize.x, windowSize.x);
	ypos = MathUtils::Clip(ypos, -windowSize.y, windowSize.y);
	glfwSetCursorPos(this->window, xpos, ypos);

	if (unintializedMouse)
	{
		mouseLastX = xpos;
		mouseLastY = ypos;
		unintializedMouse = false;
	}

	float xoffset = xpos - mouseLastX;
	float yoffset = ypos - mouseLastY; // reversed since y-coordinates range from bottom to top
	mouseLastX = xpos;
	mouseLastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	//CameraManager::GetInstance()->AdjustYaw(xoffset);
	//CameraManager::GetInstance()->AdjustPitch(yoffset);

	this->mousePosition = vec2(xpos, ypos);

	Model.SetCursorPosition(vec2(mousePosition.x/windowSize.x, mousePosition.y / windowSize.y));
}

void InputManager::SetWindow(GLFWwindow* window)
{
	this->window = window;
	this->UpdateWindowSize();
}

void InputManager::UpdateWindowSize()
{
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	this->windowSize = vec2(float(width), float(height));
	Model.Resize();
}

void InputManager::VerifyMouseCollision()
{
	vec2 convertedMousePosition = vec2((this->mousePosition.x + windowSize.x) / 2.0, (windowSize.y - this->mousePosition.y) / 2.0);
	InterfaceElement* IE = InterfaceManager::GetInstance()->GetElementAt(convertedMousePosition);

	//printf("\n MOUSE POS: %f %f", convertedMousePosition.x, convertedMousePosition.y);

	if (InterfaceManager::GetInstance()->GetMouseHoverElement() != IE)
	{
		if (InterfaceManager::GetInstance()->GetMouseHoverElement() != NULL)
			InterfaceManager::GetInstance()->GetMouseHoverElement()->Interact(InterfaceInteraction::MOUSE_LEAVE);

		InterfaceManager::GetInstance()->SetMouseHoverElement(IE);

		if (InterfaceManager::GetInstance()->GetMouseHoverElement() != NULL)
			InterfaceManager::GetInstance()->GetMouseHoverElement()->Interact(InterfaceInteraction::MOUSE_HOVER);
	}
}

void InputManager::RenderCursor()
{
	Model.Render();
}