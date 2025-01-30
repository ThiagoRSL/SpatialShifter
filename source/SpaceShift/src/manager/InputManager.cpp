
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
		PlayerShip->SetAutonomous(false);
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationZ(-1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationZ(0.0f);
		break;
	case (GLFW_KEY_D):
		PlayerShip->SetAutonomous(false);
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationZ(1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationZ(0.0f);
		break;
	case (GLFW_KEY_Q):
		PlayerShip->SetAutonomous(false);
		if (action == GLFW_PRESS)
			PlayerShip->AdjustRotationX(-1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->AdjustRotationX(0.0f);
		break;
	case (GLFW_KEY_E):
		PlayerShip->SetAutonomous(false);
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
			StageManager::GetInstance()->SetKillCount(10);
		}
		//else if (action == GLFW_RELEASE)				
		break;
	case (GLFW_KEY_W):
		PlayerShip->SetAutonomous(false);
		if (action == GLFW_PRESS)
			PlayerShip->SetThrustMode(1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->SetThrustMode(0.0f);
		break;
	case (GLFW_KEY_S):
		PlayerShip->SetAutonomous(false);
		if (action == GLFW_PRESS)
			PlayerShip->SetThrustMode(-1.0f);
		else if (action == GLFW_RELEASE)
			PlayerShip->SetThrustMode(0.0f);
		break;
	case (GLFW_KEY_LEFT_SHIFT):
		if (action == GLFW_PRESS)
		{
			CommandingAutonomy = true;
			if (PlayerShip->GetAutonomy() != NULL)
			{
				if (!PlayerShip->IsAutonomous()) PlayerShip->SetAutonomous(true);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			CommandingAutonomy = false;
			if (PlayerShip->GetAutonomy() != NULL)
			{
				if (!PlayerShip->IsAutonomous()) PlayerShip->SetAutonomous(true);
				else if (PlayerShip->IsAutonomous())
				{
					ShutdownAutonomyTask* t = new ShutdownAutonomyTask(1);
					PlayerShip->GetAutonomy()->AddTaskEnd(t);
				}
			}
		}

			
	case (GLFW_KEY_LEFT_CONTROL): 
		if (action == GLFW_PRESS)
			LeftCtrPressed = true;
		else if (action == GLFW_RELEASE)
			LeftCtrPressed = false;

		break;
	case (GLFW_KEY_LEFT_ALT):
		if (action == GLFW_PRESS)
		{
			LeftAltPressed = true;
			PathPatrolSelection.clear();
		}
		else if (action == GLFW_RELEASE)
		{
			AutonomyShipModule* Autonomy = DebugEnemy->GetAutonomy();
			if (Autonomy != NULL && 2 <= PathPatrolSelection.size())
			{
				PatrolTask* t = new PatrolTask(PathPatrolSelection);
				Autonomy->AddTaskEnd(t);
			}
			PathPatrolSelection.clear();
			LeftAltPressed = false;
		}
		break;

	case (GLFW_KEY_SPACE):
		if (action == GLFW_PRESS)
		{
			if (LeftAltPressed)
			{
				if (DebugEnemy->GetAutonomy() != NULL)
				{
					FollowTask* t = new FollowTask(PlayerShip);
					DebugEnemy->GetAutonomy()->AddTaskStart(t);
				}				
			}
		}
		else if (action == GLFW_RELEASE)
		{
			printf("\n Rot X: %f, Y: %f and Z: %f", PlayerShip->GetDirection().x, PlayerShip->GetDirection().y, PlayerShip->GetDirection().z);
		}
		break;
	}
}
void InputManager::ProcessMouse(double deltaTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		vec3 target = CameraManager::GetInstance()->WorldPivot() +
			CoordinateTranslateUtils::ScreenToWorld(mousePosition.x, mousePosition.y, windowSize.x, windowSize.y,
				CameraInstance->ViewMatrix(), CameraInstance->ProjectionMatrix(), PlayerShip->GetPosition().z);

		if (!lastPressed)
		{
			lastPressed = true;

			if (LeftAltPressed)
			{
				AutonomyShipModule* Autonomy = DebugEnemy->GetAutonomy();
				if (Autonomy != NULL)
				{
					SeekTask* t = new SeekTask(target);
					Autonomy->AddTaskEnd(t);
				}
				return;
			}

			if (InterfaceManager::GetInstance()->GetMouseHoverElement() != NULL)
			{
				InterfaceManager::GetInstance()->GetMouseHoverElement()->Interact(InterfaceInteraction::MOUSE_CLICK_LEFT);
			}
			else
			{
				if (CommandingAutonomy)
				{
					if (PlayerShip->IsAutonomous() && PlayerShip->GetAutonomy() != NULL)
					{
						SeekTask* t = new SeekTask(target);
						PlayerShip->GetAutonomy()->AddTaskEnd(t);
					}
				}
				else
				{
					PlayerShip->ShotAt(target);
				}
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
		vec3 target = CameraManager::GetInstance()->WorldPivot() +
			CoordinateTranslateUtils::ScreenToWorld(mousePosition.x, mousePosition.y, windowSize.x, windowSize.y,
				CameraInstance->ViewMatrix(), CameraInstance->ProjectionMatrix(), PlayerShip->GetPosition().z);
		
		if (!lastPressed2)
		{
			lastPressed2 = true;
			if (LeftAltPressed)
			{
				PathPatrolSelection.push_back(target);
				return;
			}

			if (InterfaceManager::GetInstance()->GetMouseHoverElement() != NULL)
			{
				InterfaceManager::GetInstance()->GetMouseHoverElement()->Interact(InterfaceInteraction::MOUSE_CLICK_RIGHT);
			}
			else
			{
				target.z = PlayerShip->GetPosition().z;

				PlayerShip->UseSkill(target);
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