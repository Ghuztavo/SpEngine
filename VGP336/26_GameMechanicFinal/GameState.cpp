#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;



void GameState::Initialize() 
{
	mLevelFile = L"../../Assets/Templates/Levels/FinalLevel.json";

	mGameWorld.LoadLevel(mLevelFile);

}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);

	// if I press ctrl the camera moves, right now the speed of the camera is 0
	
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
