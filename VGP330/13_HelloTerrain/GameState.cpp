#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;


void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 21.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	// Characters
	mCharacter.Initialize(L"Character01/Character01.model");
	mCharacter.transform.position = { 0.0f, 20.0f, 0.0f };
	mCharacter2.Initialize(L"Character02/Character02.model");
	mCharacter2.transform.position = { -2.0f, 20.0f, 0.0f };
	mCharacter3.Initialize(L"Character03/Character03.model");
	mCharacter3.transform.position = { 2.0f, 20.0f, 0.0f };

	// Shapes
	// Sphere 1
	Mesh planet = MeshBuilder::CreateSphere(30, 30, 2.0f);
	mRenderObject.meshBuffer.Initialize(planet);
	TextureManager* tm = TextureManager::Get();
	mRenderObject.diffuseMapId = tm->LoadTexture(L"earth.jpg");
	mRenderObject.specMapId = tm->LoadTexture(L"earth_spec.jpg");
	mRenderObject.normalMapId = tm->LoadTexture(L"earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture(L"earth_bump.jpg");
	mRenderObject.transform.position = { 0.0f, 25.0f, 3.0f };

	// Sphere 2
	Mesh rock = MeshBuilder::CreateSphere(30, 30, 1.0f);
	mRenderObject2.meshBuffer.Initialize(rock);
	TextureManager* tm2 = TextureManager::Get();
	mRenderObject2.normalMapId = tm2->LoadTexture(L"rock/rock_normal.jpg");
	mRenderObject2.diffuseMapId = tm2->LoadTexture(L"rock/rock_diffuse.jpg");
	mRenderObject2.transform.position = { -2.0f, 25.0f, 0.0f };

	// Sphere 3
	Mesh ground = MeshBuilder::CreateSphere(30, 30, 0.5f);
	mRenderObject3.meshBuffer.Initialize(ground);
	TextureManager* tm3 = TextureManager::Get();
	mRenderObject3.normalMapId = tm3->LoadTexture(L"ground/ground_normal.jpg");
	mRenderObject3.diffuseMapId = tm3->LoadTexture(L"ground/ground_diffuse.jpg");
	mRenderObject3.transform.position = { 2.0f, 25.0f, 0.0f };

	mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_512x512.raw", 20.0f);
	mGround.meshBuffer.Initialize(mTerrain.mesh);
	mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"terrain/dirt_seamless.jpg");
	mGround.specMapId = TextureManager::Get()->LoadTexture(L"terrain/grass_2048.jpg");


	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	mTerrainEffect.Initialize();
	mTerrainEffect.SetCamera(mCamera);
	mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mTerrainEffect.SetDirectionalLight(mDirectionalLight);
	mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());
}

void GameState::Terminate()
{
	mTerrainEffect.Terminate();
	mShadowEffect.Terminate();
	mCharacter.Terminate();
	mCharacter2.Terminate();
	mCharacter3.Terminate();
	mRenderObject.Terminate();
	mRenderObject2.Terminate();
	mRenderObject3.Terminate();
	mGround.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	mShadowEffect.Begin();
		mShadowEffect.Render(mCharacter);
		mShadowEffect.Render(mCharacter2);
		mShadowEffect.Render(mCharacter3);
		mShadowEffect.Render(mRenderObject);
		mShadowEffect.Render(mRenderObject2);
		mShadowEffect.Render(mRenderObject3);
	mShadowEffect.End();

	mTerrainEffect.Begin();
		mTerrainEffect.Render(mGround);
	mTerrainEffect.End();

	mStandardEffect.Begin();
		mStandardEffect.Render(mCharacter);
		mStandardEffect.Render(mCharacter2);
		mStandardEffect.Render(mCharacter3);
		mStandardEffect.Render(mRenderObject);
		mStandardEffect.Render(mRenderObject2);
		mStandardEffect.Render(mRenderObject3);
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
	if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (uint32_t i = 0; i < mCharacter.renderObjects.size(); ++i)
		{
			Material& material = mCharacter.renderObjects[i].material;
			std::string renderObjectId = "RenderObject " + std::to_string(i);
			ImGui::PushID(renderObjectId.c_str());
			if (ImGui::CollapsingHeader(renderObjectId.c_str()))
			{
				ImGui::LabelText("label", "Material:");
				ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
				ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
				ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
				ImGui::ColorEdit4("Specular#Material", &material.specular.r);
				ImGui::DragFloat("Shininess#Material", &material.shininess, 0.1f, 0.1f, 1000.0f);
			}
			ImGui::PopID();
		}
		
	}
	ImGui::Separator();
	ImGui::DragFloat3("CharacterPosition", &mCharacter.transform.position.x);
	
	mStandardEffect.DebugUI();
	mShadowEffect.DegubUI();
	mTerrainEffect.DebugUI();

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

	Math::Vector3 cameraPosition = mCamera.GetPosition();
	float height = mTerrain.GetHeight(cameraPosition);
	if (height > 0.0f)
	{
		cameraPosition.y = height + 1.5f;
		mCamera.SetPosition(cameraPosition);
	}
}