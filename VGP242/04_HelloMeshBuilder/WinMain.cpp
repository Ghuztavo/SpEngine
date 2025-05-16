#include <SpEngine/Inc/SpEngine.h>
#include "ShapeState.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	SpEngine::AppConfig config;
	config.appName = L"Hello Mesh Builder";

	SpEngine::App& myApp = SpEngine::MainApp();
	myApp.AddState<ShapeState>("ShapeState");
	myApp.Run(config);

	return 0;
}