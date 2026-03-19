#include "GameState.h"
#include "GameEvent.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;
using namespace SpEngine::Physics;
using namespace SpEngine::Audio;

void GameState::Initialize() 
{
	mCamera.SetPosition({ -7.0f, 3.5f, 5.0f });
	mCamera.SetLookAt({ -2.0f, 0.0f, -2.0f });

	mDirectionalLight.direction = Math::Normalize({ 0.444f, 0.206f, -0.872f });

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

	// Load Sounds
	SoundEffectManager* sm = SoundEffectManager::Get();
	mWalkingSound = sm->Load("Footsteps.wav");
	mElevatorFallingSound = sm->Load("ElevatorFalling.wav");
	mElevatorCrashSound = sm->Load("ElevatorCrashing.wav");
	mGangnamSound = sm->Load("GangnamStyle.wav");
	mBrickSound = sm->Load("Brick.wav");

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
	// Sound for character 1
	mCharacter1AnimationTime = 0.0f;
	auto startWalkingSound = [this]()
		{
			SoundEffectManager::Get()->Play(mWalkingSound, true);
		};

	auto stopWalkingSound = [this]()
		{
			SoundEffectManager::Get()->Stop(mWalkingSound);
		};
	// Character 1 Animation
	mCharacter1Animation = AnimationBuilder()
		.AddPositionKey({ 0.0f, 0.0f, 30.0f }, 0.0f)
		.AddEventKey(startWalkingSound, 0.1f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 10.0f)
		.AddEventKey(stopWalkingSound, 10.0f)
		.AddRotationKey(Math::Quaternion::Identity, 13.0f)
		.AddPositionKey({ 0.0f, 0.0f, 0.0f }, 14.0f)
		.AddRotationKey(Math::Quaternion::CreateFromAxisAngle(Math::Vector3::YAxis, 90.0f * Math::Constants::DegToRad), 14.0f)
		.AddPositionKey({ -2.0f, 0.0f, 0.0f }, 15.0f)
		.AddRotationKey(Math::Quaternion::Identity, 15.0f)
		.Build();

	// Character 2
	// Character 2 model
	mm = ModelManager::Get();
	mCharacter2.Initialize(L"Character02/Character02.model");
	mCharacter2.transform.position = { 0.0f, -3.0f, -5.0f };
	mCharacter2.transform.rotation = Math::Quaternion::CreateFromYawPitchRoll(Math::Constants::Pi, 0.0f, 0.0f);
	// Character 2 animator
	mCharacter2.animator = &mCharacter2Animator;
	mm->AddAnimation(mCharacter2.modelId, L"../../Assets/Models/Character02/Animations/GangnamStyle2.animset");
	mCharacter2Animator.Initialize(mCharacter2.modelId);
	// Sound for character 2
	auto startGangnamSound = [this]()
		{
			SoundEffectManager::Get()->Play(mGangnamSound, true);
		};

	auto stopGangnamSound = [this]()
		{
			SoundEffectManager::Get()->Stop(mGangnamSound);
		};

	auto startBrickSound = [this]()
		{
			SoundEffectManager::Get()->Play(mBrickSound, true);
		};
	auto stopBrickSound = [this]()
		{
			SoundEffectManager::Get()->Stop(mBrickSound);
		};
	// Character 2 Animation
	mCharacter2AnimationTime = 0.0f;
	mCharacter2Animation = AnimationBuilder()
		.AddEventKey(stopGangnamSound, 0.0f)
		.AddPositionKey({ -2.0f, -3.0f, -8.0f }, 0.0f)
		.AddPositionKey({ -2.0f, -3.0f, -8.0f }, 32.0f)
		.AddPositionKey({ -2.0f,  0.0f, -8.0f }, 34.0f)// Character 2 get up
		.AddPositionKey({ -2.0f,  0.0f, -6.0f }, 46.0f)// Character 2 walk forward
		.AddEventKey(startBrickSound, 45.0f)
		.AddEventKey(stopBrickSound, 49.0f)
		.AddPositionKey({ -2.0f,  0.0f, -4.0f }, 49.0f)// Character 2 start Gangnam Style dance
		.AddEventKey(startGangnamSound, 49.0f)
		.AddRotationKey(Math::Quaternion::CreateFromYawPitchRoll(Math::Constants::Pi, 0.0f, 0.0f), 0.0f)
		.Build();

	// Elevator 
	// Elevator button panel
	MeshPC buttonPanel = MeshBuilder::CreateRectanglePC(0.5f, 1.5f, 0.3f);
	mElevatorButtonPanel.meshBuffer.Initialize(buttonPanel);
	mElevatorButtonPanel.transform.position = { -0.05f, 0.8f, -0.65f };

	// Elevator button
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

	// Elevator
	auto startContainerFalling = [this]()
		{
			SoundEffectManager::Get()->Play(mElevatorFallingSound, true);
		};
	auto stopContainerFalling = [this]()
		{
			SoundEffectManager::Get()->Stop(mElevatorFallingSound);
		};
	mm = ModelManager::Get();
	mElevator.Initialize(L"Container/Container.model");
	mElevatorAnimationTime = 0.0f;
	mElevatorAnimation = AnimationBuilder()
		.AddPositionKey({ -2.0f, 500.0f, -10.0f }, 0.0f)
		.AddScaleKey({ 500.0f, 10.0f, 10.0f }, 0.0f)
		.AddRotationKey(Math::Quaternion::CreateFromYawPitchRoll(Math::Constants::HalfPi, -Math::Constants::HalfPi,0.0f), 0.0f)
		.AddPositionKey({ -2.0f, 500.0f, -10.0f }, 25.0f)
		.AddEventKey(startContainerFalling, 27.0f)
		.AddEventKey(stopContainerFalling, 32.0f)
		.AddPositionKey({ -2.0f, 4.2f, -10.0f }, 32.0f)
		.Build();

	// Particles
	ParticleSystemInfo info;
	info.textureId = TextureManager::Get()->LoadTexture("Images/stone.png");
	info.maxParticles = 100;
	info.particlesPerEmit = { 20, 30 };
	info.delay = 0.0f;
	info.lifeTime = 0.0f;
	info.timeBetweenEmits = { 0.1f, 0.2f };
	info.spawnAngle = { -180.0f, 180.0f };
	info.spawnSpeed = { 7.0f, 15.0f };
	info.particleLifeTime = { 0.5f, 1.5f };
	info.spawnDirection = Math::Vector3::YAxis;
	info.spawnPosition = Math::Vector3::Zero;
	info.startScale = { Math::Vector3::One, Math::Vector3::One };
	info.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
	info.startColor = { Graphics::Colors::White, Graphics::Colors::White };
	info.endColor = { Graphics::Colors::White, Graphics::Colors::White };
	mCrashingParticles.Initialize(info);

	auto containerCrash = [&]()
		{
			mCrashingParticles.SpawnParticles();
			SoundEffectManager::Get()->Play(mElevatorCrashSound);
		};
	mCrashingAnimationTime = 0.0f;
	mCrashingAnimation = AnimationBuilder()
		.AddPositionKey({ -2.0f, 0.0f, -5.0f }, 0.0f)
		.AddPositionKey({ -2.0f, 0.0f, -5.0f }, 32.0f)
		.AddEventKey(containerCrash, 32.0f)
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

	// Elevator/Container
	mElevatorButtonPanel.Terminate();
	mElevatorButton.Terminate();
	mElevator.Terminate();

	mParticleSystemEffect.Terminate();
	mCrashingParticles.Terminate();

	SoundEffectManager* sm = SoundEffectManager::Get();
	sm->Stop(mWalkingSound);
	sm->Stop(mElevatorFallingSound);
	sm->Stop(mElevatorCrashSound);
	sm->Stop(mGangnamSound);
	sm->Stop(mBrickSound);
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	// Update character 1 animation
	mCharacter1Animator.Update(deltaTime);
	mCharacter1AnimationTime += deltaTime;
	mCharacter1Animation.PlayEvents(mCharacter1AnimationTime - deltaTime, mCharacter1AnimationTime);
	UpdateCharacter1Animator(mCharacter1AnimationTime);

	// Update character 2 animation
	mCharacter2Animator.Update(deltaTime);
	mCharacter2AnimationTime += deltaTime;
	mCharacter2Animation.PlayEvents(mCharacter2AnimationTime - deltaTime, mCharacter2AnimationTime);
	UpdateCharacter2Animator(mCharacter2AnimationTime);
	
	// Update elevator animation
	mElevatorButtonAnimationTime += deltaTime;
	mElevatorAnimation.PlayEvents(mElevatorAnimationTime, mElevatorAnimationTime + deltaTime);
	mElevatorAnimationTime += deltaTime;

	mCrashingParticles.Update(deltaTime);
	mCrashingAnimation.PlayEvents(mCrashingAnimationTime, mCrashingAnimationTime + deltaTime);
	mCrashingAnimationTime += deltaTime;
	
}

void GameState::Render()
{
	// Render sky sphere
	mSimpleTextureEffect.Begin();
	mSimpleTextureEffect.Render(mSkySphere);
	mSimpleTextureEffect.End();

	// Update transforms for characters and elevator
	mCharacter1.transform = mCharacter1Animation.GetTransform(mCharacter1AnimationTime);
	mCharacter2.transform = mCharacter2Animation.GetTransform(mCharacter2AnimationTime);

	mElevatorButton.transform = mElevatorButtonAnimation.GetTransform(mElevatorButtonAnimationTime);
	mElevator.transform = mElevatorAnimation.GetTransform(mElevatorAnimationTime);

	Transform particleTransform = mCrashingAnimation.GetTransform(mCrashingAnimationTime);
	mCrashingParticles.SetPosition(particleTransform.position);

	// Render ground and characters
	mStandardEffect.Begin();
		mStandardEffect.Render(mGroundObject);
		mStandardEffect.Render(mCharacter1);
		mStandardEffect.Render(mCharacter2);
		mStandardEffect.Render(mElevatorButtonPanel);
		mStandardEffect.Render(mElevatorButton);
		mStandardEffect.Render(mElevator);
	mStandardEffect.End();

	// Render particles
	mParticleSystemEffect.Begin();
	mCrashingParticles.Render(mParticleSystemEffect);
	mParticleSystemEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Animation Time: %.2f", mCharacter1AnimationTime);

	ImGui::Separator();

	if (ImGui::Button("Reset Animation"))
	{
		// Reset animation timelines
		mCharacter1AnimationTime = 0.0f;
		mCharacter2AnimationTime = 0.0f;
		mElevatorButtonAnimationTime = 0.0f;
		mElevatorAnimationTime = 0.0f;
		mCrashingAnimationTime = 0.0f;

		// Reset anitor clip indices
		mCharacter1ClipIndex = -1;
		mCharacter2ClipIndex = -1;

		// Stop sounds
		SoundEffectManager* sm = SoundEffectManager::Get();
		sm->Stop(mWalkingSound);
		sm->Stop(mGangnamSound);
		sm->Stop(mElevatorFallingSound);
		sm->Stop(mElevatorCrashSound);
		sm->Stop(mBrickSound);
	}
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
	else if (mCharacter1ClipIndex == 3 && animationTime >= 37.0f) // getting up
	{
		mCharacter1Animator.PlayAnimation(4, false);
		mCharacter1ClipIndex = 4;
	}
	else if (mCharacter1ClipIndex == 4 && animationTime >= 41.0f) // idle
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
	if (mCharacter2ClipIndex == -1 && animationTime <= 1)
	{
		mCharacter2Animator.PlayAnimation(-1, true);
	}
	if (mCharacter2ClipIndex < 0 && animationTime >= 49.0f) // Gangnam Style
	{
		mCharacter2Animator.PlayAnimation(0, true);
		mCharacter2ClipIndex = 0;
	}
}

