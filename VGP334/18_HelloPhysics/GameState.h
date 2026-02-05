#pragma once

#include <SpEngine/Inc/SpEngine.h>

class GameState : public SpEngine::AppState 
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:

	void UpdateCamera(float deltaTime);

	SpEngine::Graphics::Camera mCamera;
	SpEngine::Graphics::DirectionalLight mDirectionalLight;

	SpEngine::Graphics::RenderObject mBallObject;
	SpEngine::Physics::CollisionShape mBallShape;
	SpEngine::Physics::RigidBody mBallRigidBody;

	SpEngine::Graphics::RenderObject mGroundObject;
	SpEngine::Physics::CollisionShape mGroundShape;
	SpEngine::Physics::RigidBody mGroundRigidBody;

	SpEngine::Graphics::StandardEffect mStandardEffect;

	
};