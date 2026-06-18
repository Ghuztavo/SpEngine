#include "GameState.h"
#include "WeaponComponent.h"
#include "WeaponService.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;

Service* MakeWeaponService(const std::string& serviceName, GameWorld& gameWorld)
{
	if (serviceName == "WeaponService")
	{
		return gameWorld.AddService<WeaponService>();
	}

	return nullptr;
}

Component* MakeWeaponComponent(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "WeaponComponent")
	{
		return gameObject.AddComponent<WeaponComponent>();
	}
	return nullptr;
}

Component* GetWeaponComponent(const std::string& componentName, GameObject& gameObject)
{
	if (componentName == "WeaponComponent")
	{
		return gameObject.GetComponent<WeaponComponent>();
	}
	return nullptr;
}

void GameState::Initialize() 
{
	mLevelFile = L"../../Assets/Templates/Levels/FinalLevel.json";

	GameWorld::SetCustomService(MakeWeaponService);

	GameObjectFactory::SetCustomMake(MakeWeaponComponent);
	GameObjectFactory::SetCustomGet(GetWeaponComponent);

	mGameWorld.LoadLevel(mLevelFile);

}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI();
	if (ImGui::Button("ReloadLevel"))
	{
		mGameWorld.Terminate();
		mGameWorld.LoadLevel(mLevelFile);
	}
	ImGui::End();

}
