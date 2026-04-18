#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;


void GameState::Initialize() 
{
	mGameWorld.Initialize();

	GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
	mCameraComponent = cameraGO->AddComponent<CameraComponent>();
	cameraGO->AddComponent<FPSCameraComponent>();
	cameraGO->Initialize();

	GameObject* enemyGO = mGameWorld.CreateGameObject("Enemy");
	enemyGO->AddComponent<TransformComponent>();
	enemyGO->Initialize();

	GameObject* playerGO = mGameWorld.CreateGameObject("Player");
	TransformComponent* playerTransform = playerGO->AddComponent<TransformComponent>();
	playerTransform->position.x = 2.0f;
	playerGO->Initialize();

	GameObject* weaponGo = mGameWorld.CreateGameObject("Weapon");
	TransformComponent* weaponTransform = weaponGo->AddComponent<TransformComponent>();
	weaponTransform->position.x = -2.0f;
	weaponGo->Initialize();

	GameObject* obstacleGO = mGameWorld.CreateGameObject("Obstacle");
	TransformComponent* obstacleTransform = obstacleGO->AddComponent<TransformComponent>();
	obstacleTransform->position = { 0.0f, 0.0f, 2.0f };
	obstacleGO->Initialize();

	GameObject* vehicleGO = mGameWorld.CreateGameObject("Vehicle");
	TransformComponent* vehicleTransform = vehicleGO->AddComponent<TransformComponent>();
	vehicleTransform->position = { 0.0f, 0.0f, -2.0f };
	vehicleGO->Initialize();
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
	ImGui::End();

	SimpleDraw::Render(mCameraComponent->GetCamera());
}
