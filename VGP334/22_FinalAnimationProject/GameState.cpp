#include "GameState.h"
#include "GameEvent.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;
using namespace SpEngine::Audio;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mSimpleTextureEffect.Initialize();
	mSimpleTextureEffect.SetCamera(mCamera);

	mParticleSystemEffect.Initialize();
	mParticleSystemEffect.SetCamera(mCamera);

	// Create sky sphere
	MeshPX sky = MeshBuilder::CreateSkyBoxSpherePX(60, 60, 300.0f);
	mSkySphere.mesh.Initialize(sky);
	mSkySphere.textureId = TextureManager::Get()->LoadTexture(L"skysphere/sky.jpg");

	// Create ground plane
	TextureManager* tm = TextureManager::Get();
	Mesh plane = MeshBuilder::CreatePlane(100, 100, 1.0f, true);
	mGroundObject.meshBuffer.Initialize(plane);
	mGroundObject.diffuseMapId = tm->LoadTexture("misc/concrete.jpg");
	mGroundShape.InitializeHull({ 100.0f, 0.5f, 100.0f }, { 0.0f, -0.5f, 0.0f });
	mGroundRigidBody.Initialize(mGroundObject.transform, mGroundShape);

	// Character 1
	// Character 1 model
	ModelManager* mm = ModelManager::Get();
	mCharacter1.Initialize(L"Character01/Character01.model");
	mCharacter1.transform.position = { 0.0f, 0.0f, 30.0f };
	// Character 1 animator
	mCharacter1.animator = &mCharacter1Animator;
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/Walking.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/ButtonPushing.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/Bored.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/FallingDown.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/GettingUp.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/Idle.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/Surprised.animset");
	mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Models/Character01/Animations/GangnamStyle.animset");
	mCharacter1Animator.Initialize(mCharacter1.modelId);
	// Character 1 Animation
	mCharacter1AnimationTime = 0.0f;
	mCharacter1Animation = AnimationBuilder()
		.AddPositionKey({ 0.0f, 0.0f, 30.0f }, 0.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 10.0f)
		.AddRotationKey(Math::Quaternion::Identity, 13.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 14.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90.0f * Math::Constants::DegToRad), 14.0f)
		.AddPositionKey({ -2.0f, 0.0f, 0.0f }, 15.0f)
		.AddRotationKey(Math::Quaternion::Identity, 15.0f)
		.Build();

	// Character 2
	// Character 2 model
	ModelManager* mm2 = ModelManager::Get();
	mCharacter2.Initialize(L"Character02/Character02.model");
	mCharacter2.transform.position = { 0.0f, -3.0f, -5.0f };
	// Better to use Quaternion to apply yaw rotation (PI = 180 degrees mapping)
	mCharacter2.transform.rotation = Math::Quaternion::CreateFromYawPitchRoll(Math::Constants::Pi, 0.0f, 0.0f);
	// Character 2 animator
	mCharacter2.animator = &mCharacter2Animator;
	mm2->AddAnimation(mCharacter2.modelId, L"../../Assets/Models/Character02/Animations/GangnamStyle2.animset");
	mCharacter2Animator.Initialize(mCharacter2.modelId);
	// Character 2 Animation
	mCharacter2AnimationTime = 0.0f;
	mCharacter2Animation = AnimationBuilder()
		.AddPositionKey({ -2.0f, -3.0f, -5.0f }, 0.0f)
		.AddPositionKey({ -2.0f, -3.0f, -5.0f }, 46.0f)
		.AddPositionKey({ -2.0f,  0.0f, -5.0f }, 49.0f)
		.AddRotationKey(Math::Quaternion::CreateFromYawPitchRoll(Math::Constants::Pi, 0.0f, 0.0f), 0.0f)
		.Build();

	// Elevator 
	//Elevator button
	MeshPC buttonPanel = MeshBuilder::CreateRectanglePC(0.5f, 1.5f, 0.1f);
	mElevatorButtonPanel.meshBuffer.Initialize(buttonPanel);
	mElevatorButtonPanel.transform.position = { -0.1f, 0.8f, -0.5f };

	//Button
	Mesh button = MeshBuilder::CreateSphere(20, 20, 0.1f);
	mElevatorButton.meshBuffer.Initialize(button);
	mElevatorButton.diffuseMapId = tm->LoadTexture(L"planets/venus.jpg");
	mElevatorButton.transform.position = { -0.02f, 1.2f, -0.5f };
	mElevatorButtonAnimationTime = 0.0f;
	mElevatorButtonAnimation = AnimationBuilder()
		.AddPositionKey({ -0.02f, 1.2f, -0.5f }, 11.8f)
		.AddPositionKey({ -0.02f, 1.2f, -0.6f }, 12.0f)
		.AddPositionKey({ -0.02f, 1.2f, -0.5f }, 12.2f)
		.Build();



}

void GameState::Terminate()
{
	mParticleSystemEffect.Terminate();
	mStandardEffect.Terminate();
	mSimpleTextureEffect.Terminate();
	TextureManager::Get()->ReleaseTexture(mSkySphere.textureId);
	mGroundRigidBody.Terminate();
	mGroundShape.Terminate();
	mGroundObject.meshBuffer.Terminate();
	mGroundObject.Terminate();
	mSkySphere.mesh.Terminate();

	// Character 1
	mCharacter1.Terminate();

	// Character 2
	mCharacter2.Terminate();

	mElevatorButtonPanel.Terminate();
	mElevatorButton.Terminate();
	
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	// Update character 1 animation
	mCharacter1Animator.Update(deltaTime);
	mCharacter1AnimationTime += deltaTime;
	UpdateCharacter1Animator(mCharacter1AnimationTime);

	// Update character 2 animation
	mCharacter2Animator.Update(deltaTime);
	mCharacter2AnimationTime += deltaTime;
	UpdateCharacter2Animator(mCharacter2AnimationTime);
	
	// Update elevator button animation
	mElevatorButtonAnimationTime += deltaTime;
}

void GameState::Render()
{
	// Render sky sphere
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mSkySphere);
	mSimpleTextureEffect.End();

	mCharacter1.transform = mCharacter1Animation.GetTransform(mCharacter1AnimationTime);
	mCharacter2.transform = mCharacter2Animation.GetTransform(mCharacter2AnimationTime);

	mElevatorButton.transform = mElevatorButtonAnimation.GetTransform(mElevatorButtonAnimationTime);

	mStandardEffect.Begin();
		mStandardEffect.Render(mGroundObject);
		mStandardEffect.Render(mCharacter1);
		mStandardEffect.Render(mCharacter2);
		mStandardEffect.Render(mElevatorButtonPanel);
		mStandardEffect.Render(mElevatorButton);
	mStandardEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	
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

}

void GameState::UpdateCharacter1Animator(float animationTime)
{
	if (mCharacter1ClipIndex < 0) // walking
	{
		mCharacter1Animator.PlayAnimation(0, true);
		mCharacter1ClipIndex = 0;
	}
	else if (mCharacter1ClipIndex == 0 && animationTime >= 10.0f) // button pushing
	{
		mCharacter1Animator.PlayAnimation(1, false);
		mCharacter1ClipIndex = 1;
	}
	else if (mCharacter1ClipIndex == 1 && animationTime >= 17.0f) // bored
	{
		mCharacter1Animator.PlayAnimation(2, true);
		mCharacter1ClipIndex = 2;
	}
	else if (mCharacter1ClipIndex == 2 && animationTime >= 32.0f) // falling down
	{
		mCharacter1Animator.PlayAnimation(3, false);
		mCharacter1ClipIndex = 3;
	}
	else if (mCharacter1ClipIndex == 3 && animationTime >= 35.0f) // getting up
	{
		mCharacter1Animator.PlayAnimation(4, false);
		mCharacter1ClipIndex = 4;
	}
	else if (mCharacter1ClipIndex == 4 && animationTime >= 39.0f) // idle
	{
		mCharacter1Animator.PlayAnimation(5, true);
		mCharacter1ClipIndex = 5;
	}
	else if (mCharacter1ClipIndex == 5 && animationTime >= 47.0f) // surprised
	{
		mCharacter1Animator.PlayAnimation(6, true);
		mCharacter1ClipIndex = 6;
	}
	else if (mCharacter1ClipIndex == 6 && animationTime >= 52.0f) // gangnam style
	{
		mCharacter1Animator.PlayAnimation(7, true);
		mCharacter1ClipIndex = 7;
	}
	
}

void GameState::UpdateCharacter2Animator(float animationTime)
{
	if (mCharacter2ClipIndex < 0 && animationTime >= 49.0f) // Gangnam Style
	{
		mCharacter2Animator.PlayAnimation(0, true);
		mCharacter2ClipIndex = 0;
	}
}

