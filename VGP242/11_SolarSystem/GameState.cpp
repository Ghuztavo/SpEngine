#include "GameState.h"
#include <iostream>

using namespace SpEngine;
using namespace SpEngine::Graphics;
using namespace SpEngine::Input;

enum class PlanetRenderTarget
{
	Sun,
	Mercury,
	Venus,
	Earth,
	Moon,
	Mars,
	Jupiter,
	Saturn,
	Uranus,
	Neptune,
	Pluto
};
PlanetRenderTarget gCurrentPlanet = PlanetRenderTarget::Sun;
const char* gPlanetNames[] = {
	"Sun", "Mercury", "Venus", "Earth", "Moon", "Mars",
	"Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"
};

void GameState::Initialize() 
{
	mCamera.SetPosition({ 0.0f, 1.0f, -10.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -10.0f });
	mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	mRenderTargetCamera.SetAspectRatio(1.0f);

	// initialize gpu communication
	mSimpleTextureEffect.Initialize();

	//initialize render object
	//space
	MeshPX spaceMesh = MeshBuilder::CreateSkyBoxSpherePX(60, 60, 200.0f);
	space.mesh.Initialize(spaceMesh);
	space.textureId = TextureManager::Get()->LoadTexture(L"space.jpg");

	//sun
	MeshPX sunMesh = MeshBuilder::CreateSpherePX(60, 60, 5.0f);
	sun.mesh.Initialize(sunMesh);
	sun.textureId = TextureManager::Get()->LoadTexture(L"sun.jpg");

	//mercury
	mercury.radius = 0.3f;
	mercury.centerObject = sun.matWorld;
	mercury.distanceFromCenter = 8.0f;
	mercury.orbitTranslation = 1.0f;
	mercury.rotationOnAxis = 1.0f;
	MeshPX mercuryMesh = MeshBuilder::CreateSpherePX(60, 60, mercury.radius);
	mercury.renderData.mesh.Initialize(mercuryMesh);
	mercury.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/mercury.jpg");
	mercury.renderData.matWorld = Math::Matrix4::Translation(mercury.distanceFromCenter, 0, 0);

	//venus
	venus.radius = 0.5f;
	venus.centerObject = sun.matWorld;
	venus.distanceFromCenter = 10.0f;
	venus.orbitTranslation = 0.9f;
	venus.rotationOnAxis = 1.0f;
	MeshPX venusMesh = MeshBuilder::CreateSpherePX(60, 60, venus.radius);
	venus.renderData.mesh.Initialize(venusMesh);
	venus.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/venus.jpg");
	venus.renderData.matWorld = Math::Matrix4::Translation(venus.distanceFromCenter, 0, 0);

	//earth
	earth.radius = 0.5f;
	earth.centerObject = sun.matWorld;
	earth.distanceFromCenter = 12.0f;
	earth.orbitTranslation = 0.8f;
	earth.rotationOnAxis = 1.0f;
	MeshPX earthMesh = MeshBuilder::CreateSpherePX(60, 60, earth.radius);
	earth.renderData.mesh.Initialize(earthMesh);
	earth.renderData.textureId = TextureManager::Get()->LoadTexture(L"earth.jpg");
	earth.renderData.matWorld = Math::Matrix4::Translation(earth.distanceFromCenter, 0, 0);

	//moon
	moon.radius = 0.1f;
	moon.centerObject = earth.renderData.matWorld;
	moon.distanceFromCenter = 0.75f;
	moon.orbitTranslation = 0.2f;
	moon.rotationOnAxis = 1.0f;
	MeshPX moonMesh = MeshBuilder::CreateSpherePX(60, 60, moon.radius);
	moon.renderData.mesh.Initialize(moonMesh);
	moon.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/pluto.jpg"); //there is no moon texture so I used pluto texture
	moon.renderData.matWorld = Math::Matrix4::Translation(earth.distanceFromCenter + moon.distanceFromCenter, 0, 0);

	//mars
	mars.radius = 0.5f;
	mars.centerObject = sun.matWorld;
	mars.distanceFromCenter = 14.0f;
	mars.orbitTranslation = 0.7f;
	mars.rotationOnAxis = 1.0f;
	MeshPX marsMesh = MeshBuilder::CreateSpherePX(60, 60, mars.radius);
	mars.renderData.mesh.Initialize(marsMesh);
	mars.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/mars.jpg");
	mars.renderData.matWorld = Math::Matrix4::Translation(mars.distanceFromCenter, 0, 0);
	
	//jupiter
	jupiter.radius = 2.0f;
	jupiter.centerObject = sun.matWorld;
	jupiter.distanceFromCenter = 18.0f;
	jupiter.orbitTranslation = 0.6f;
	jupiter.rotationOnAxis = 1.0f;
	MeshPX jupiterMesh = MeshBuilder::CreateSpherePX(60, 60, jupiter.radius);
	jupiter.renderData.mesh.Initialize(jupiterMesh);
	jupiter.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/jupiter.jpg");
	jupiter.renderData.matWorld = Math::Matrix4::Translation(jupiter.distanceFromCenter, 0, 0);

	//saturn
	saturn.radius = 1.5f;
	saturn.centerObject = sun.matWorld;
	saturn.distanceFromCenter = 24.0f;
	saturn.orbitTranslation = 0.5f;
	saturn.rotationOnAxis = 1.0f;
	MeshPX saturnMesh = MeshBuilder::CreateSpherePX(60, 60, saturn.radius);
	saturn.renderData.mesh.Initialize(saturnMesh);
	saturn.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/saturn.jpg");
	saturn.renderData.matWorld = Math::Matrix4::Translation(saturn.distanceFromCenter, 0, 0);

	//uranus
	uranus.radius = 0.7f;
	uranus.centerObject = sun.matWorld;
	uranus.distanceFromCenter = 28.0f;
	uranus.orbitTranslation = 0.4f;
	uranus.rotationOnAxis = 1.0f;
	MeshPX uranusMesh = MeshBuilder::CreateSpherePX(60, 60, uranus.radius);
	uranus.renderData.mesh.Initialize(uranusMesh);
	uranus.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/uranus.jpg");
	uranus.renderData.matWorld = Math::Matrix4::Translation(uranus.distanceFromCenter, 0, 0);

	//neptune
	neptune.radius = 0.7f;
	neptune.centerObject = sun.matWorld;
	neptune.distanceFromCenter = 32.0f;
	neptune.orbitTranslation = 0.3f;
	neptune.rotationOnAxis = 1.0f;
	MeshPX neptuneMesh = MeshBuilder::CreateSpherePX(60, 60, neptune.radius);
	neptune.renderData.mesh.Initialize(neptuneMesh);
	neptune.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/neptune.jpg");
	neptune.renderData.matWorld = Math::Matrix4::Translation(neptune.distanceFromCenter, 0, 0);

	//pluto
	pluto.radius = 0.2f;
	pluto.centerObject = sun.matWorld;
	pluto.distanceFromCenter = 36.0f;
	pluto.orbitTranslation = 0.2f;
	pluto.rotationOnAxis = 1.0f;
	MeshPX plutoMesh = MeshBuilder::CreateSpherePX(60, 60, pluto.radius);
	pluto.renderData.mesh.Initialize(plutoMesh);
	pluto.renderData.textureId = TextureManager::Get()->LoadTexture(L"planets/pluto.jpg");
	pluto.renderData.matWorld = Math::Matrix4::Translation(pluto.distanceFromCenter, 0, 0);

	constexpr uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);

}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	TextureManager::Get()->ReleaseTexture(sun.textureId);
	TextureManager::Get()->ReleaseTexture(space.textureId);
	TextureManager::Get()->ReleaseTexture(mercury.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(venus.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(earth.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(moon.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(mars.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(jupiter.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(saturn.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(uranus.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(neptune.renderData.textureId);
	TextureManager::Get()->ReleaseTexture(pluto.renderData.textureId);
	sun.mesh.Terminate();
	space.mesh.Terminate();
	mercury.renderData.mesh.Terminate();
	venus.renderData.mesh.Terminate();
	earth.renderData.mesh.Terminate();
	moon.renderData.mesh.Terminate();
	mars.renderData.mesh.Terminate();
	jupiter.renderData.mesh.Terminate();
	saturn.renderData.mesh.Terminate();
	uranus.renderData.mesh.Terminate();
	neptune.renderData.mesh.Terminate();
	pluto.renderData.mesh.Terminate();
	mSimpleTextureEffect.Terminate();
}

Math::Vector3 GetObjectPosition(Math::Matrix4& centerObject)
{
	return Math::Vector3(
		centerObject._41, // X position
		centerObject._42, // Y position
		centerObject._43  // Z position
	);
}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
	UpdatePlanet(mercury, deltaTime);
	UpdatePlanet(venus, deltaTime);
	UpdatePlanet(earth, deltaTime);
	UpdateMoon(moon, earth, deltaTime);
	UpdatePlanet(mars, deltaTime);
	UpdatePlanet(jupiter, deltaTime);
	UpdatePlanet(saturn, deltaTime);
	UpdatePlanet(uranus, deltaTime);
	UpdatePlanet(neptune, deltaTime);
	UpdatePlanet(pluto, deltaTime);

	//update the render target camera to look at the selected planet
	if (gCurrentPlanet == PlanetRenderTarget::Sun)
	{
		UpdateRenderTargetCamera(deltaTime, Math::Vector3::Zero, 11);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Mercury)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(mercury.renderData.matWorld), mercury.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Venus)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(venus.renderData.matWorld), venus.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Earth)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(earth.renderData.matWorld), earth.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Moon)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(moon.renderData.matWorld), moon.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Mars)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(mars.renderData.matWorld), mars.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Jupiter)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(jupiter.renderData.matWorld), jupiter.radius + 2.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Saturn)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(saturn.renderData.matWorld), saturn.radius + 2.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Uranus)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(uranus.renderData.matWorld), uranus.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Neptune)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(neptune.renderData.matWorld), neptune.radius + 1.0f);
	}
	else if (gCurrentPlanet == PlanetRenderTarget::Pluto)
	{
		UpdateRenderTargetCamera(deltaTime, GetObjectPosition(pluto.renderData.matWorld), pluto.radius + 1.0f);
	}
}

void GameState::Render()
{
	SimpleDraw::Render(mCamera);

	//render to render targettar
	mSimpleTextureEffect.SetCamera(mRenderTargetCamera);
	mRenderTarget.BeginRender();
		mSimpleTextureEffect.Begin();
			if (gCurrentPlanet == PlanetRenderTarget::Sun)
		{
			mSimpleTextureEffect.Render(sun);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Mercury)
		{
			mSimpleTextureEffect.Render(mercury.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Venus)
		{
			mSimpleTextureEffect.Render(venus.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Earth)
		{
			mSimpleTextureEffect.Render(earth.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Moon)
		{
			mSimpleTextureEffect.Render(moon.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Mars)
		{
			mSimpleTextureEffect.Render(mars.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Jupiter)
		{
			mSimpleTextureEffect.Render(jupiter.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Saturn)
		{
			mSimpleTextureEffect.Render(saturn.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Uranus)
		{
			mSimpleTextureEffect.Render(uranus.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Neptune)
		{
			mSimpleTextureEffect.Render(neptune.renderData);
		}
			else if (gCurrentPlanet == PlanetRenderTarget::Pluto)
		{
			mSimpleTextureEffect.Render(pluto.renderData);
		}
		mSimpleTextureEffect.End();
	mRenderTarget.EndRender();

	//render to the scene
	mSimpleTextureEffect.SetCamera(mCamera);
	mSimpleTextureEffect.Begin();
		mSimpleTextureEffect.Render(sun);
		mSimpleTextureEffect.Render(space);
		mSimpleTextureEffect.Render(mercury.renderData);
		mSimpleTextureEffect.Render(venus.renderData);
		mSimpleTextureEffect.Render(earth.renderData);
		mSimpleTextureEffect.Render(moon.renderData);
		mSimpleTextureEffect.Render(mars.renderData);
		mSimpleTextureEffect.Render(jupiter.renderData);
		mSimpleTextureEffect.Render(saturn.renderData);
		mSimpleTextureEffect.Render(uranus.renderData);
		mSimpleTextureEffect.Render(neptune.renderData);
		mSimpleTextureEffect.Render(pluto.renderData);
	mSimpleTextureEffect.End();
}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Solar System");

	ImGui::Checkbox("Show Orbit Rings", &orbitRings);
	if (orbitRings)
	{
		SimpleDraw::AddGroundCircle(60, mercury.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, venus.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, earth.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, mars.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, jupiter.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, saturn.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, uranus.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, neptune.distanceFromCenter, Colors::White, Math::Vector3::Zero);
		SimpleDraw::AddGroundCircle(60, pluto.distanceFromCenter, Colors::White, Math::Vector3::Zero);
	}

	ImGui::Text("Choose planet to render and edit");
	int currentPlanet = (int)gCurrentPlanet;
	if (ImGui::Combo("Planet", &currentPlanet, gPlanetNames, std::size(gPlanetNames)))
	{
		gCurrentPlanet = static_cast<PlanetRenderTarget>(currentPlanet);
	}

	switch (gCurrentPlanet)
	{
	case PlanetRenderTarget::Sun:
		ImGui::Text("Sun");

		break;
	case PlanetRenderTarget::Mercury:
		ImGui::Text("Mercury");
		ImGui::DragFloat("Orbit Translation Speed", &mercury.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Venus:
		ImGui::Text("Venus");
		ImGui::DragFloat("Orbit Translation Speed", &venus.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Earth:
		ImGui::Text("Earth");
		ImGui::DragFloat("Orbit Translation Speed", &earth.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Moon:
		ImGui::Text("Moon");
		ImGui::DragFloat("Orbit Translation Speed", &moon.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Mars:
		ImGui::Text("Mars");
		ImGui::DragFloat("Orbit Translation Speed", &mars.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Jupiter:
		ImGui::Text("Jupiter");
		ImGui::DragFloat("Orbit Translation Speed", &jupiter.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Saturn:
		ImGui::Text("Saturn");
		ImGui::DragFloat("Orbit Translation Speed", &saturn.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Uranus:
		ImGui::Text("Uranus");
		ImGui::DragFloat("Orbit Translation Speed", &uranus.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Neptune:
		ImGui::Text("Neptune");
		ImGui::DragFloat("Orbit Translation Speed", &neptune.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	case PlanetRenderTarget::Pluto:
		ImGui::Text("Pluto");
		ImGui::DragFloat("Orbit Translation Speed", &pluto.orbitTranslation, 0.01f, 0.0f, 10.0f);
		break;
	default:
		break;
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

void GameState::UpdatePlanet(SpaceObject& planet, float deltaTime)
{
	// Advance orbit angle
	planet.orbitAngle += planet.rotationOnAxis * deltaTime;
	if (planet.orbitAngle >= Math::Constants::TwoPi)
		planet.orbitAngle -= Math::Constants::TwoPi;

	// Advance self-rotation angle
	planet.selfRotationAngle += planet.orbitTranslation * deltaTime;
	if (planet.selfRotationAngle >= Math::Constants::TwoPi)
		planet.selfRotationAngle -= Math::Constants::TwoPi;

	// Compute transforms
	const auto orbitRotation = Math::Matrix4::RotationY(planet.orbitAngle);
	const auto orbitTranslation = Math::Matrix4::Translation(planet.distanceFromCenter, 0.0f, 0.0f);
	const auto orbitTransform = orbitRotation * orbitTranslation;

	const auto selfRotation = Math::Matrix4::RotationY(planet.selfRotationAngle);

	// Apply orbit, then parent, then spin
	planet.renderData.matWorld = orbitTransform * planet.centerObject * selfRotation;
}

void GameState::UpdateMoon(SpaceObject& moon, const SpaceObject& earth, float deltaTime)
{
	// Update orbit and self-rotation angles
	moon.orbitAngle += moon.orbitTranslation * deltaTime;
	if (moon.orbitAngle >= Math::Constants::TwoPi)
		moon.orbitAngle -= Math::Constants::TwoPi;

	moon.selfRotationAngle += moon.rotationOnAxis * deltaTime;
	if (moon.selfRotationAngle >= Math::Constants::TwoPi)
		moon.selfRotationAngle -= Math::Constants::TwoPi;

	// Orbit transform (around Earth)
	auto orbitRotation = Math::Matrix4::RotationY(moon.orbitAngle);
	auto orbitTranslation = Math::Matrix4::Translation(moon.distanceFromCenter, 0.0f, 0.0f);
	auto orbitTransform = orbitTranslation * orbitRotation;

	// Self rotation
	auto selfRotation = Math::Matrix4::RotationY(moon.selfRotationAngle);
	
	// Final transformation: rotate on self -> orbit Earth -> use Earth’s position only
	moon.renderData.matWorld = selfRotation * orbitTransform * earth.renderData.matWorld;
}

void GameState::UpdateRenderTargetCamera(float deltaTime, Math::Vector3 targetPosition, float objectRadius)
{
	if (gCurrentPlanet == PlanetRenderTarget::Sun)
	{
		mRenderTargetCamera.SetPosition({ 0.0f, 1.0f, -10.0f });
		mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	}
	else
	{
		mRenderTargetCamera.SetPosition(targetPosition + Math::Vector3(0.0f, 1.0f, -objectRadius));
		mRenderTargetCamera.SetLookAt(targetPosition);
	}
}