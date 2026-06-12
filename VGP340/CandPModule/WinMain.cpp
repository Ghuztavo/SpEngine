#include "CandPApp.h"
#include "GameState.h"

using namespace SpEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Concurrency and Parallel Programming";

	CandPApp myApp;
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}