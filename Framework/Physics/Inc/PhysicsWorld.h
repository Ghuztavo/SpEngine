#pragma once

namespace SpEngine::Physics
{
	class PhysicsObject;
	class PhysicsWorld final
	{
	public:
		struct Settings
		{
			// is the gravity used for physics simulations
			Math::Vector3 gravity = { 0.0f, -9.81f, 0.0f };
			// if update loops are too slow, this is the maximum simulations physics will run
			uint32_t simulationSteps = 1;
			// the fixed rate that the simulation swill run to ensure consistent/predictable outcomes
			float fixedTimeStep = 1.0f / 60.0f;
		};

		static void StaticInitialize(const Settings& settings);
		static void StaticTerminate();
		static PhysicsWorld* Get();

		PhysicsWorld() = default;
		~PhysicsWorld();

		void Initialize(const Settings& settings);
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		void SetGravity(const Math::Vector3& gravity);

		void Register(PhysicsObject* physicsObject);
		void Unregister(PhysicsObject* physicsObject);

	private:
		Settings mSettings;

		//bullet objects
		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
		btSequentialImpulseConstraintSolver* mSolver = nullptr;
		// this is the physics world that runs the simulations
		btDiscreteDynamicsWorld* mDynamicsWorld = nullptr;

		using PhysicsObjects = std::vector<PhysicsObject*>;
		PhysicsObjects mPhysicsObjects;
	};
}