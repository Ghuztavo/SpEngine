#include <SpEngine/Inc/SpEngine.h>
#include "ShapeState.h"


int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	SpEngine::AppConfig config;
	config.appName = L"Hello Shapes";

	SpEngine::App& myApp = SpEngine::MainApp();
	myApp.AddState<ShapeState>("ShapeState");
	myApp.AddState<TriforceShapeState>("TriforceShapeState");
	myApp.AddState<HeartShapeState>("HeartShapeState");
	myApp.Run(config);

	return 0;
}