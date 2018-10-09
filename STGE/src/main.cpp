#include "Core/Window.h"
#include "Input/Input.h"
#include <time.h>

#include "Scenes/MainScene.h"



float left = -10, right = 10, bottom = -10, top = 10;

int main()
{
	srand(time(NULL));

	Window win(1280, 720, "TITLE?!");
	win.SetWorldBounds({ left, top, right, bottom });

	Input::SetupCallbacks(&win);

	float ratio = (float)win.GetWidth() / (float)win.GetHeight();
	left *= ratio;
	right *= ratio;
	Shader::GetBasic()->UniformMat4f("projectionMat", glm::ortho(left, right, bottom, top, -0.5f, 100.0f));
	
	MainScene scene(win);
	scene.Setup();

	while (win.IsOpen())
	{
		scene.Update();
		scene.Draw();
	}

	return 0;
}