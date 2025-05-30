#include "GameState.h"

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;

enum class SphereRenderTarget
{
	Sun,
	Earth,
	Void
};
SphereRenderTarget gCurrentSphere = SphereRenderTarget::Sun;

const char* gSphereNames[] = { "Sun", "Earth", "Void"};

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	// initialize gpu communication
	std::filesystem::path shaderFile = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderFile);
	mPixelShader.Initialize(shaderFile);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
	mTransformBuffer.Initialize(sizeof(Math::Matrix4));

	//initialize render object
	MeshPX sphere = MeshBuilder::CreateSpherePX(60, 60, 1.0f);
	mObject.meshBuffer.Initialize(sphere);
	mObject2.meshBuffer.Initialize(sphere);
	mObject3.meshBuffer.Initialize(sphere);


	mObject.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");
	mObject2.textureId = TextureManager::Get()->LoadTexture(L"earth.jpg");
	mObject3.textureId = TextureManager::Get()->LoadTexture(L"space.jpg");
	mObject2.worldMat = Math::Matrix4::Translation(4.0f, 0.0f, 0.0f);
	mObject3.worldMat = Math::Matrix4::Translation(-4.0f, 0.0f, 0.0f);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);

}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	TextureManager::Get()->ReleaseTexture(mObject.textureId);
	TextureManager::Get()->ReleaseTexture(mObject2.textureId);
	TextureManager::Get()->ReleaseTexture(mObject3.textureId);
	mObject.meshBuffer.Terminate();
	mObject2.meshBuffer.Terminate();
	mObject3.meshBuffer.Terminate();
	mTransformBuffer.Terminate();
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}

void GameState::Render()
{
	SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);

	//render to render target
	mRenderTarget.BeginRender();
	if (gCurrentSphere == SphereRenderTarget::Sun)
	{
		RenderObject(mObject, mRenderTargetCamera);
		mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
		mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	}
	else if (gCurrentSphere == SphereRenderTarget::Earth)
	{
		RenderObject(mObject2, mRenderTargetCamera);
		mRenderTargetCamera.SetPosition({ 4.0f, 1.0f, -3.0f });
		mRenderTargetCamera.SetLookAt({ 4.0f, 0.0f, 0.0f });
	}
	else if (gCurrentSphere == SphereRenderTarget::Void)
	{
		RenderObject(mObject3, mRenderTargetCamera);
		mRenderTargetCamera.SetPosition({ -4.0f, 1.0f, -3.0f });
		mRenderTargetCamera.SetLookAt({ -4.0f, 0.0f, 0.0f });
	}
	else
	{
		RenderObject(mObject, mRenderTargetCamera);
	}
	mRenderTarget.EndRender();

	//render the scene
	RenderObject(mObject, mCamera);
	RenderObject(mObject2, mCamera);
	RenderObject(mObject3, mCamera);
}

void GameState::RenderObject(const Object& object, const SpEngine::Graphics::Camera& camera)
{
	const Math::Matrix4 matView = camera.GetViewMatrix();
	const Math::Matrix4 matProj = camera.GetProjectionMatrix();
	const Math::Matrix4 matFinal = object.worldMat * matView * matProj;
	const Math::Matrix4 wvp = Math::Transpose(matFinal);
	mTransformBuffer.Update(&wvp);

	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	mTransformBuffer.BindVs(0);

	TextureManager::Get()->BindPS(object.textureId, 0);
	object.meshBuffer.Render();
}

void GameState::DebugUI()
{
	
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Choose object to render");

	int currentSphere = (int)gCurrentSphere;
	if (ImGui::Combo("Planet", &currentSphere, gSphereNames, std::size(gSphereNames)))
	{
		gCurrentSphere = (SphereRenderTarget)currentSphere;
	}

	ImGui::Separator();
	ImGui::Text("Render Target");
	ImGui::Image(
		mRenderTarget.GetRawData(),
		{ 128, 128 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1, 1, 1 },
		{ 1, 1, 1, 1 });
		

	ImGui::End();

	/*SimpleDraw::AddGroundPlane(20.0f, Colors::White);
	SimpleDraw::Render(mCamera);*/
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