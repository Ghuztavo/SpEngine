#include <SpEngine/Inc/SpEngine.h>

class MainState : public SpEngine::AppState
{
public:
	void Initialize()
	{
		LOG("MAIN STATE INITIALIZED");
		mLifeTime = 2.0f;
	}
	void Terminate()override
	{
		LOG("MAIN STATE TERMINATED");
	}
	void Update(float deltaTime) override
	{
		mLifeTime -= std::max(deltaTime, 0.01f);
		if (mLifeTime <= 0.0f)
		{
			SpEngine::MainApp().ChangeState("GameState");
		}
	}
private:
	float mLifeTime = 0.0f;
};

class GameState :public SpEngine::AppState
{
public:
	void Initialize() override
	{
		LOG("GAME STATE INITIALIZED");
		mLifeTime = 2.0f;
	}
	void Terminate()override
	{
		LOG("MAIN STATE TERMINATED");
	}
	void Update(float deltaTime) override
	{
		mLifeTime -= std::max(deltaTime, 0.01f);
		if (mLifeTime <= 0.0f)
		{
			SpEngine::MainApp().ChangeState("MainState");
		}
	}

private:
	float mLifeTime = 0.0f;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	SpEngine::AppConfig config;
	config.appName = L"Hello Window";

	SpEngine::App& myApp = SpEngine::MainApp();
	myApp.AddState<MainState>("MainState");
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}