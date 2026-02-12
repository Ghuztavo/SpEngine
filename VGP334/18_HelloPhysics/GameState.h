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

	struct BoxData
	{
		SpEngine::Graphics::RenderObject box;
		SpEngine::Physics::CollisionShape shape;
		SpEngine::Physics::RigidBody rigidBody;
	};
	using Boxes = std::vector<BoxData>;
	Boxes mBoxes;

	SpEngine::Graphics::StandardEffect mStandardEffect;

	// cloth info
	SpEngine::Graphics::RenderObject mCloth;
	SpEngine::Graphics::Mesh mClothMesh;
	SpEngine::Physics::SoftBody mClothSoftBody;
};