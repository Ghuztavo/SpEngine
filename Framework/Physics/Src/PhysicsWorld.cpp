#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace SpEngine;
using namespace SpEngine::Physics;

namespace
{
	std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
	ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld: is already initialized");
	sPhysicsWorld = std::make_unique<PhysicsWorld>();
	sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
	if ( sPhysicsWorld != nullptr )
	{
		sPhysicsWorld->Terminate();
		sPhysicsWorld.reset();
	}
}

PhysicsWorld* PhysicsWorld::Get()
{
	ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld: must be initialized");
	return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
	ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld: must be terminated");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
	mSettings = settings;
	mInterface = new btDbvtBroadphase();
	mSolver = new btSequentialImpulseConstraintSolver();
#ifdef USE_SOFT_BODY
	mCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mDynamicsWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
#else
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
#endif

	mDynamicsWorld->setGravity(TobtVector3(mSettings.gravity));
	mDynamicsWorld->setDebugDrawer(&mPhysicsDebugDraw);
}

void PhysicsWorld::Terminate()
{
	SafeDelete(mDynamicsWorld);
	SafeDelete(mDispatcher);
	SafeDelete(mCollisionConfiguration);
	SafeDelete(mSolver);
	SafeDelete(mInterface);
}

void PhysicsWorld::Update(float deltaTime)
{
	mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
	for (PhysicsObject* obj : mPhysicsObjects)
	{
		obj->SyncWithGraphics();
	}
}
void PhysicsWorld::DebugUI()
{
	if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat("Gravity", &mSettings.gravity.x, 0.1f))
		{
			mDynamicsWorld->setGravity(TobtVector3(mSettings.gravity));
		}
		ImGui::Checkbox("DebugDraw", &mDebugDraw);
		if (mDebugDraw)
		{
			ImGui::Indent();

			int debugMode = mPhysicsDebugDraw.getDebugMode();
			bool drawWireFrame = (debugMode & btIDebugDraw::DBG_DrawWireframe);
			if (ImGui::Checkbox("DrawWireFrame", &drawWireFrame))
			{
				debugMode = (drawWireFrame) ? (debugMode | btIDebugDraw::DBG_DrawWireframe) : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
			}
			bool drawAABB = (debugMode & btIDebugDraw::DBG_DrawAabb);
			if(ImGui::Checkbox("DrawAABB", &drawAABB))
			{
				debugMode = (drawAABB) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
			}
			bool drawContactPoints = (debugMode & btIDebugDraw::DBG_DrawContactPoints);
			if(ImGui::Checkbox("DrawContactPoints", &drawContactPoints))
			{
				debugMode = (drawContactPoints) ? debugMode | btIDebugDraw::DBG_DrawContactPoints : debugMode & ~btIDebugDraw::DBG_DrawContactPoints;
			}
			mPhysicsDebugDraw.setDebugMode(debugMode);
			mDynamicsWorld->debugDrawWorld();

			ImGui::Unindent();
		}
	}
}

void PhysicsWorld::UpdateSettings(const Settings& settings)
{
	mSettings = settings;
	SetGravity(settings.gravity);
}

void PhysicsWorld::SetGravity(const Math::Vector3& gravity)
{
	mSettings.gravity = gravity;
	mDynamicsWorld->setGravity(TobtVector3(gravity));
}

const PhysicsWorld::Settings& PhysicsWorld::GetSettings() const
{
	return mSettings;
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	//if iter is the end, it is NOT in the list, Register means we want to add it, so it is safe to add now
	if (iter == mPhysicsObjects.end())
	{
		mPhysicsObjects.push_back(physicsObject);
#ifdef USE_SOFT_BODY
		if (physicsObject->GetSoftBody() != nullptr)
		{
			mDynamicsWorld->addSoftBody(physicsObject->GetSoftBody());
		}
#endif
		if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
		}
	}
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
	auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
	//if iter is NOT the end, it IS in the list, unregister means we want to remove it, we found it so we can remove it now
	if (iter != mPhysicsObjects.end())
	{
#ifdef USE_SOFT_BODY
		if (physicsObject->GetSoftBody() != nullptr)
		{
			mDynamicsWorld->removeSoftBody(physicsObject->GetSoftBody());
		}
#endif
		if (physicsObject->GetRigidBody() != nullptr)
		{
			mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
		}
		mPhysicsObjects.erase(iter);
	}
}

PhysicsWorld::RaycastResult PhysicsWorld::Raycast(const Math::Vector3& origin, const Math::Vector3& direction, float distance)
{
	RaycastResult result;
	if (mDynamicsWorld == nullptr)
	{
		return result;
	}

	Math::Vector3 end = origin + (direction * distance);
	btVector3 btFrom = TobtVector3(origin);
	btVector3 btTo = TobtVector3(end);

	btCollisionWorld::ClosestRayResultCallback callback(btFrom, btTo);
	mDynamicsWorld->rayTest(btFrom, btTo, callback);

	if (callback.hasHit())
	{
		result.hasHit = true;
		result.hitPoint = ToVector3(callback.m_hitPointWorld);
		result.hitNormal = ToVector3(callback.m_hitNormalWorld);
	}

	return result;
}


struct IgnoreBodyCallback : public btCollisionWorld::ClosestRayResultCallback
{
	const btCollisionObject* mIgnoreBody;

	IgnoreBodyCallback(const btVector3& from, const btVector3& to, const btCollisionObject* ignore)
		: btCollisionWorld::ClosestRayResultCallback(from, to)
		, mIgnoreBody(ignore)
	{}

	btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) override
	{
		if (rayResult.m_collisionObject == mIgnoreBody)
		{
			return 1.0f;
		}
		return btCollisionWorld::ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
	}
};

PhysicsWorld::RaycastResult PhysicsWorld::Raycast(const Math::Vector3& origin, const Math::Vector3& direction, float distance, const btCollisionObject* ignoreBody)
{
	RaycastResult result;
	if (mDynamicsWorld == nullptr)
	{
		return result;
	}

	Math::Vector3 end = origin + (direction * distance);
	btVector3 btFrom = TobtVector3(origin);
	btVector3 btTo = TobtVector3(end);

	IgnoreBodyCallback callback(btFrom, btTo, ignoreBody);
	mDynamicsWorld->rayTest(btFrom, btTo, callback);

	if (callback.hasHit())
	{
		result.hasHit = true;
		result.hitPoint = ToVector3(callback.m_hitPointWorld);
		result.hitNormal = ToVector3(callback.m_hitNormalWorld);
	}

	return result;
}
