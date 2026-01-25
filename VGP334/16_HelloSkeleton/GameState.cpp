#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 2.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	mCharacter.Initialize(L"Character01/Character01.model");
	mCharacter2.Initialize(L"Character02/Character02.model");
	mCharacter2.transform.position = { -2.0f, 0.0f, 0.0f };
	mCharacter3.Initialize(L"Character03/Character03.model");
	mCharacter3.transform.position = { 2.0f, 0.0f, 0.0f };

	mSkeletonRender = CharacterSkeleton::None;

	std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderFile);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
	mCharacter.Terminate();
	mCharacter2.Terminate();
	mCharacter3.Terminate();
	mStandardEffect.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	if (mDrawSkeleton)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::BoneTransforms boneTransforms2;
		AnimationUtil::BoneTransforms boneTransforms3;
		switch (mSkeletonRender)
		{
		case CharacterSkeleton::Character01:
			AnimationUtil::ComputeBoneTransforms(mCharacter.modelId, boneTransforms);
			AnimationUtil::DrawSkeleton(mCharacter.modelId, boneTransforms);
			break;
		case CharacterSkeleton::Character02:
			AnimationUtil::ComputeBoneTransforms(mCharacter2.modelId, boneTransforms2);
			AnimationUtil::DrawSkeleton(mCharacter2.modelId, boneTransforms2);
			break;
		case CharacterSkeleton::Character03:
			AnimationUtil::ComputeBoneTransforms(mCharacter3.modelId, boneTransforms3);
			AnimationUtil::DrawSkeleton(mCharacter3.modelId, boneTransforms3);
			break;
		case CharacterSkeleton::None:
			break;
		}
			SimpleDraw::AddGroundPlane(20.0f, Colors::White);
			SimpleDraw::Render(mCamera);
	}
	else
	{
		SimpleDraw::AddGroundPlane(20.0f, Colors::White);
		SimpleDraw::Render(mCamera);

		mStandardEffect.Begin();
		mStandardEffect.Render(mCharacter);
		mStandardEffect.Render(mCharacter2);
		mStandardEffect.Render(mCharacter3);
		mStandardEffect.End();
	}
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

	ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);

	ImGui::Text("Select Character Skeleton to Render:");
	ImGui::RadioButton("Scifi character", (int*)&mSkeletonRender, (int)CharacterSkeleton::Character01);
	ImGui::RadioButton("Knight character", (int*)&mSkeletonRender, (int)CharacterSkeleton::Character02);
	ImGui::RadioButton("Monster character", (int*)&mSkeletonRender, (int)CharacterSkeleton::Character03);
	ImGui::RadioButton("None", (int*)&mSkeletonRender, (int)CharacterSkeleton::None);

	
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