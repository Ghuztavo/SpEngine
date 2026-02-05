#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;


void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	// sphere 1
	Mesh mesh = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mBallObject.meshBuffer.Initialize(mesh);
	TextureManager* tm = TextureManager::Get();
	mBallObject.diffuseMapId = tm->LoadTexture(L"misc/basketball.jpg");
	mBallObject.transform.position.y = 5.0f;
	mBallShape.InitializeSphere(1.0f);
	mBallRigidBody.Initialize(mBallObject.transform, mBallShape, 1.0f);

	Mesh plane = MeshBuilder::CreatePlane(10.0f, 10.0f, 1.0f, true);
	mGroundObject.meshBuffer.Initialize(plane);
	mGroundObject.diffuseMapId = tm->LoadTexture(L"misc/concrete.jpg");
	mGroundShape.InitializeHull({ 5.0f, 0.5f, 5.0f }, { 0.0f, -0.5f, 0.0f });
	mGroundRigidBody.Initialize(mGroundObject.transform, mGroundShape);

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);

}

void GameState::Terminate()
{
	mStandardEffect.Terminate();

	mBallRigidBody.Terminate();
	mBallShape.Terminate();
	mBallObject.Terminate();

	mGroundRigidBody.Terminate();
	mGroundShape.Terminate();
	mGroundObject.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{	
	mStandardEffect.Begin();
	mStandardEffect.Render(mBallObject);
	mStandardEffect.Render(mGroundObject);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Direction#Light", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}

		ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
	}

	ImGui::Separator();

	if (ImGui::CollapsingHeader("Material_Of_Planet", ImGuiTreeNodeFlags_DefaultOpen))
	{

		ImGui::ColorEdit4("Emissive#Material", &mBallObject.material.emissive.r);
		ImGui::ColorEdit4("Ambient#Material", &mBallObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse#Material", &mBallObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular#Material", &mBallObject.material.specular.r);
		ImGui::DragFloat("Shininess#Material", &mBallObject.material.shininess, 0.01f, 0.01f, 10000.0f);
	}

	Math::Vector3 pos = mBallObject.transform.position;
	if (ImGui::DragFloat3("BallPosition", &pos.x))
	{
		mBallObject.transform.position = pos;
		mBallRigidBody.SetPosition(mBallObject.transform.position);
	}
	
	mStandardEffect.DebugUI();
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	Input::InputSystem* input = Input::InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(Input::KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 1.0f;

	if (input->IsKeyDown(Input::KeyCode::W)) mCamera.Walk(moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::S)) mCamera.Walk(-moveSpeed * deltaTime);

	if (input->IsKeyDown(Input::KeyCode::A)) mCamera.Strafe(-moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::D)) mCamera.Strafe(moveSpeed * deltaTime);

	if (input->IsKeyDown(Input::KeyCode::Q)) mCamera.Rise(-moveSpeed * deltaTime);
	else if (input->IsKeyDown(Input::KeyCode::E)) mCamera.Rise(moveSpeed * deltaTime);

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
	//salmon was here puto el que lo lea
}