#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;


void GameState::Initialize() 
{
	mLevelFile = L"../../Assets/Templates/Levels/CandPLevel.json";

	mGameWorld.LoadLevel(mLevelFile);
	mAi.Start(mGameWorld);
}

void GameState::Terminate()
{
	mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
	mGameWorld.Update(deltaTime);
	mAi.UpdateAgents(deltaTime);
}

void GameState::Render()
{
	mGameWorld.Render();

	mAi.DrawGrid();
	mAi.DrawAgentPaths();

	const auto* cameraService = mGameWorld.GetService<SpEngine::CameraService>();
	if (cameraService != nullptr)
	{
		SpEngine::Graphics::SimpleDraw::Render(cameraService->GetMain());
	}
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI();
	if (ImGui::Button("ReloadLevel"))
	{
		mGameWorld.Terminate();
		mGameWorld.LoadLevel(mLevelFile);
		mAi.Start(mGameWorld);
	}
	ImGui::End();

}
