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
	SpEngine::Graphics::StandardEffect mStandardEffect;


	SpEngine::Graphics::RenderObject mGroundObject;
	SpEngine::Physics::CollisionShape mGroundShape;
	SpEngine::Physics::RigidBody mGroundRigidBody;

	struct Segment
	{
		SpEngine::Graphics::RenderObject segment;
		float range = 0.0f;
	};

	std::vector<Segment> mSegments;
	SpEngine::Physics::CollisionShape mBodyAnchorShape;
	SpEngine::Physics::RigidBody mBodyAnchor;
};