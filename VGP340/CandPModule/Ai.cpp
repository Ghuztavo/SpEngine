#include "Ai.h"
#include <queue>
#include <future>
#include <chrono>
#include <atomic>

// Convert a node's grid position to a centered world space position on x z
SpEngine::Math::Vector3 Ai::NodeToWorld(const Node* node) const
{
	const float halfW = (COL_COUNT * NODE_SIZE) * 0.5f;
	const float halfH = (ROW_COUNT * NODE_SIZE) * 0.5f;

	// Place the agent at the cell centre
	const float x = (node->col + 0.5f) * NODE_SIZE - halfW;
	const float z = (node->row + 0.5f) * NODE_SIZE - halfH;
	return { x, 0.5f, z };
}

void Ai::Start(SpEngine::GameWorld& world)
{
	// Build the grid
	grid.clear();
	grid.resize(ROW_COUNT);
	for (int r = 0; r < ROW_COUNT; ++r)
	{
		grid[r].reserve(COL_COUNT);
		for (int c = 0; c < COL_COUNT; ++c)
		{
			grid[r].emplace_back(NODE_SIZE, r, c);
		}
	}

	SpawnObstacles(world);

	// Single threaded and multi-threaded versions
	
	// Single-threaded version
	//SpawnAgentsSingleThreaded(world); // <------------------------------------------------------------------------- single-threaded version, the debug log shows the time

	// Multi-threaded version
	SpawnAgentsMultiThreaded(world); // <---------------------------------------------------------------------------- multi-threaded version, the debug log shows the time

	// note: the path finding is faster with multi-threading but the overall time is a bit faster with the single-threaded version because of all the allocations of the agent tasks, maybe
}

void Ai::DrawGrid()
{
	const float yOffset = 0.05f;
	const float halfW   = (COL_COUNT * NODE_SIZE) * 0.5f;
	const float halfH   = (ROW_COUNT * NODE_SIZE) * 0.5f;

	for (int r = 0; r < ROW_COUNT; ++r)
	{
		for (int c = 0; c < COL_COUNT; ++c)
		{
			const Node& node = grid[r][c];

			const float x = c * NODE_SIZE - halfW;
			const float z = r * NODE_SIZE - halfH;

			SpEngine::Math::Vector3 tl = { x,             yOffset, z             };
			SpEngine::Math::Vector3 tr = { x + NODE_SIZE, yOffset, z             };
			SpEngine::Math::Vector3 bl = { x,             yOffset, z + NODE_SIZE };
			SpEngine::Math::Vector3 br = { x + NODE_SIZE, yOffset, z + NODE_SIZE };

			SpEngine::Graphics::Color color = SpEngine::Graphics::Colors::White;
			if (node.blocked)   color = SpEngine::Graphics::Colors::Red;
			else if (node.selected) color = SpEngine::Graphics::Colors::Yellow;

			SpEngine::Graphics::SimpleDraw::AddLine(tl, tr, color);
			SpEngine::Graphics::SimpleDraw::AddLine(bl, br, color);
			SpEngine::Graphics::SimpleDraw::AddLine(tl, bl, color);
			SpEngine::Graphics::SimpleDraw::AddLine(tr, br, color);
		}
	}
}

Node* Ai::GetRandomNode()
{
	int randRow = rand() % ROW_COUNT;
	int randCol = rand() % COL_COUNT;
	return &grid[randRow][randCol];
}

void Ai::SpawnObstacles(SpEngine::GameWorld& world)
{
	const std::filesystem::path obstacleTemplate =
		L"../../Assets/Templates/Objects/simple_mesh.json";

	int placed = 0;
	int maxAttempts = OBSTACLE_COUNT * 10; // to avoid an infinite loop

	while (placed < OBSTACLE_COUNT && maxAttempts-- > 0)
	{
		Node* node = GetRandomNode();

		// skip nodes that are already blocked
		if (node->blocked)
			continue;

		// Mark the node as blocked
		node->blocked = true;

		// Spawn a cube at this node
		std::string name = "Obstacle_" + std::to_string(placed);
		SpEngine::GameObject* go = world.CreateGameObject(name, obstacleTemplate);
		if (go == nullptr)
		{
			node->blocked = false;
			continue;
		}

		go->Initialize();

		auto* transform = go->GetComponent<SpEngine::TransformComponent>();
		if (transform != nullptr)
		{
			transform->position = NodeToWorld(node);
		}

		++placed;
	}
}

//  Spawn agents Single-threaded version
void Ai::SpawnAgentsSingleThreaded(SpEngine::GameWorld& world)
{
	mAgents.clear();

	const std::filesystem::path agentTemplate =
		L"../../Assets/Templates/Objects/AiAgent_obj.json";

	double totalSearchTimeMs = 0.0;

	for (int i = 0; i < AGENT_COUNT; ++i)
	{
		Node* startNode = GetRandomNode();
		while (startNode->blocked)
			startNode = GetRandomNode();

		Node* goalNode = GetRandomNode();
		while (goalNode == startNode || goalNode->blocked)
			goalNode = GetRandomNode();

		auto t0 = std::chrono::high_resolution_clock::now();
		AStarSearch(startNode, goalNode);
		auto t1 = std::chrono::high_resolution_clock::now();
		totalSearchTimeMs += std::chrono::duration<double, std::milli>(t1 - t0).count();

		if (aStarPath.empty())
		{
			--i;   // retry this agent slot
			continue;
		}

		// Create the game object and initialize it
		std::string name = "Agent_" + std::to_string(i);
		SpEngine::GameObject* go = world.CreateGameObject(name, agentTemplate);
		if (go == nullptr)
			continue;

		go->Initialize();  // registers RigidBody with PhysicsService

		// place the object at the start node's world position
		auto* transform = go->GetComponent<SpEngine::TransformComponent>();
		if (transform != nullptr)
		{
			transform->position = NodeToWorld(startNode);
		}

		auto* rb = go->GetComponent<SpEngine::RigidBodyComponent>();
		if (rb != nullptr)
		{
			rb->SetPosition(NodeToWorld(startNode));
		}

		// Store the agent entry
		AgentEntry entry;
		entry.gameObject = go;
		entry.startNode  = startNode;
		entry.goalNode   = goalNode;

		// reverse the path 
		entry.path.assign(aStarPath.rbegin(), aStarPath.rend());
		entry.pathIndex  = 0;
		entry.moveTimer  = 0.0f;

		mAgents.push_back(std::move(entry));
	}
	
	LOG("Single-Threaded Pathfinding calculation took: %.3f ms", totalSearchTimeMs);
}

//  Spawn agents Multi-threaded version
void Ai::SpawnAgentsMultiThreaded(SpEngine::GameWorld& world)
{
	mAgents.clear();

	const std::filesystem::path agentTemplate = L"../../Assets/Templates/Objects/AiAgent_obj.json";

	// Data for async tasks
	struct AgentTask
	{
		Node* startNode = nullptr;
		Node* goalNode = nullptr;
		std::vector<Node*> path;
	};

	std::vector<AgentTask> tasks(AGENT_COUNT);

	for (int i = 0; i < AGENT_COUNT; ++i)
	{
		Node* startNode = GetRandomNode();
		while (startNode->blocked)
			startNode = GetRandomNode();

		Node* goalNode = GetRandomNode();
		while (goalNode == startNode || goalNode->blocked)
			goalNode = GetRandomNode();

		tasks[i].startNode = startNode;
		tasks[i].goalNode = goalNode;
	}

	// Determine optimal number of worker threads
	int numThreads = std::thread::hardware_concurrency();
	if (numThreads == 0) numThreads = 4;
	numThreads = std::min(numThreads, (int)AGENT_COUNT);

	int agentsPerThread = AGENT_COUNT / numThreads;
	int extraAgents = AGENT_COUNT % numThreads;

	std::vector<std::future<void>> futures;
	futures.reserve(numThreads);

	int currentIdx = 0;

	// measure only the thread dispatch and pathfinding
	auto searchStartTime = std::chrono::high_resolution_clock::now();

	for (int t = 0; t < numThreads; ++t)
	{
		int count = agentsPerThread + (t < extraAgents ? 1 : 0);
		int startIdx = currentIdx;
		int endIdx = currentIdx + count;
		currentIdx = endIdx;

		futures.push_back(std::async(std::launch::async, [this, &tasks, startIdx, endIdx]() {
			for (int i = startIdx; i < endIdx; ++i)
			{
				tasks[i].path = AStarSearchThreadSafe(tasks[i].startNode, tasks[i].goalNode);
			}
		}));
	}

	// Wait for all worker threads to finish
	for (auto& f : futures)
	{
		f.get();
	}

	auto searchEndTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> searchElapsed = searchEndTime - searchStartTime;
	LOG("Multi-Threaded Pathfinding calculation took: %.3f ms (Using %d threads)", searchElapsed.count(), numThreads);

	// Spawn GameObjects 
	for (int i = 0; i < (int)tasks.size(); ++i)
	{
		auto& task = tasks[i];
		std::vector<Node*>& path = task.path;

		if (path.empty())
			continue;

		std::string name = "Agent_" + std::to_string(i);
		SpEngine::GameObject* go = world.CreateGameObject(name, agentTemplate);
		if (go == nullptr)
			continue;

		go->Initialize();

		auto* transform = go->GetComponent<SpEngine::TransformComponent>();
		if (transform != nullptr)
		{
			transform->position = NodeToWorld(task.startNode);
		}

		auto* rb = go->GetComponent<SpEngine::RigidBodyComponent>();
		if (rb != nullptr)
		{
			rb->SetPosition(NodeToWorld(task.startNode));
		}

		AgentEntry entry;
		entry.gameObject = go;
		entry.startNode = task.startNode;
		entry.goalNode = task.goalNode;
		
		// reverse the path because A* returns it from goal to start
		entry.path.assign(path.rbegin(), path.rend());
		entry.pathIndex = 0;
		entry.moveTimer = 0.0f;

		mAgents.push_back(std::move(entry));
	}
}


void Ai::UpdateAgents(float deltaTime)
{
	for (auto& agent : mAgents)
	{
		if (agent.path.empty())
			continue;

		if (agent.pathIndex >= (int)agent.path.size())
			continue;

		auto* transform = agent.gameObject->GetComponent<SpEngine::TransformComponent>();
		auto* rb        = agent.gameObject->GetComponent<SpEngine::RigidBodyComponent>();
		if (transform == nullptr)
			continue;

		// Current target waypoint in world space
		const SpEngine::Math::Vector3 target = NodeToWorld(agent.path[agent.pathIndex]);
		const SpEngine::Math::Vector3 current = transform->position;

		// Direction and distance to next node
		SpEngine::Math::Vector3 dir = target - current;
		const float dist = SpEngine::Math::Magnitude(dir);

		const float step = AGENT_MOVE_SPEED * deltaTime;

		if (dist <= step)
		{
			transform->position = target;
			if (rb != nullptr) rb->SetPosition(target);
			++agent.pathIndex;
		}
		else
		{
			// Move toward target
			dir = SpEngine::Math::Normalize(dir);
			SpEngine::Math::Vector3 newPos = current + dir * step;
			transform->position = newPos;
			if (rb != nullptr) rb->SetPosition(newPos);
		}
	}
}

void Ai::DrawAgentPaths()
{
	// colors for the paths
	static const SpEngine::Graphics::Color kPalette[] =
	{
		{ 0.2f, 0.8f, 1.0f, 1.0f },   
		{ 1.0f, 0.5f, 0.0f, 1.0f },   
		{ 0.4f, 1.0f, 0.4f, 1.0f },   
		{ 1.0f, 0.2f, 0.8f, 1.0f },   
		{ 1.0f, 1.0f, 0.2f, 1.0f },
	};
	const int paletteSize = (int)std::size(kPalette);

	for (int i = 0; i < (int)mAgents.size(); ++i)
	{
		const auto& agent = mAgents[i];
		const auto& color = kPalette[i % paletteSize];

		for (int j = 1; j < (int)agent.path.size(); ++j)
		{
			SpEngine::Math::Vector3 from = NodeToWorld(agent.path[j - 1]);
			SpEngine::Math::Vector3 to   = NodeToWorld(agent.path[j]);
			from.y = 0.15f;
			to.y   = 0.15f;
			SpEngine::Graphics::SimpleDraw::AddLine(from, to, color);
		}
	}
}