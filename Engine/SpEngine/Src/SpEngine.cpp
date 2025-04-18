#include "Precompiled.h"
#include "SpEngine.h"

SpEngine::App& SpEngine::MainApp()
{
	static App sApp;
	return sApp;
}