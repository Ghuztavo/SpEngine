#include "Ai.h"
#include <queue>
#include <future>
#include <chrono>

// Convert a node's grid position to a centered world-space XZ coordinate
// The grid is centered at the world origin the same way DrawGrid() draws it
SpEngine::Math::Vector3 Ai::NodeToWorld(const Node* node) const
{
	const float halfW = (COL_COUNT * NODE_SIZE) * 0.5f;
	const float halfH = (ROW_COUNT * NODE_SIZE) * 0.5f;

	// Place the agent at the cell centre
	const float x = (node->col + 0.5f) * NODE_SIZE - halfW;
	const float z = (node->row + 0.5f) * NODE_SIZE - halfH;
	return { x, 0.5f, z };
}

//  Grid initialisation

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

	// Block nodes and place obstacle cubes first
	SpawnObstacles(world);

	// single threaded and multi-threaded versions
	
	auto startTime = std::chrono::high_resolution_clock::now();
	
	// Single-threaded version
	//SpawnAgentsSingleThreaded(world);

	// Multi-threaded version
	SpawnAgentsMultiThreaded(world);

	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = endTime - startTime;
	LOG("Agent spawning and A* pathfinding took: %.3f ms", elapsed.count());
}

//  Draw grid

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

//  A* random node helper

Node* Ai::GetRandomNode()
{
	int randRow = rand() % ROW_COUNT;
	int randCol = rand() % COL_COUNT;
	return &grid[randRow][randCol];
}

//  Spawn obstacle cubes at randomly blocked nodes

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
	// Clean up any leftover entries from a previous run
	mAgents.clear();

	const std::filesystem::path agentTemplate =
		L"../../Assets/Templates/Objects/AiAgent_obj.json";

	for (int i = 0; i < AGENT_COUNT; ++i)
	{
		// pick two unblocked random nodes for start and goal
		Node* startNode = GetRandomNode();
		while (startNode->blocked)
			startNode = GetRandomNode();

		Node* goalNode = GetRandomNode();
		while (goalNode == startNode || goalNode->blocked)
			goalNode = GetRandomNode();

		// Run A* to build the path
		AStarSearch(startNode, goalNode);

		// Only spawn if a path was actually found
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

		// sync the rigid body
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
}

//  Spawn agents Multi-threaded version

void Ai::SpawnAgentsMultiThreaded(SpEngine::GameWorld& world)
{
	mAgents.clear();

	const std::filesystem::path agentTemplate = L"../../Assets/Templates/Objects/AiAgent_obj.json";

	// Data needed for each agent's async task
	struct AgentTask
	{
		Node* startNode = nullptr;
		Node* goalNode = nullptr;
		std::future<std::vector<Node*>> pathFuture;
	};

	std::vector<AgentTask> tasks;
	tasks.reserve(AGENT_COUNT);

	for (int i = 0; i < AGENT_COUNT; ++i)
	{
		Node* startNode = GetRandomNode();
		while (startNode->blocked)
			startNode = GetRandomNode();

		Node* goalNode = GetRandomNode();
		while (goalNode == startNode || goalNode->blocked)
			goalNode = GetRandomNode();

		AgentTask task;
		task.startNode = startNode;
		task.goalNode = goalNode;
		// Launch A* on a background thread
		task.pathFuture = std::async(std::launch::async, &Ai::AStarSearchThreadSafe, this, startNode, goalNode);
		
		tasks.push_back(std::move(task));
	}

	// Wait for threads to finish and spawn GameObjects
	for (int i = 0; i < (int)tasks.size(); ++i)
	{
		auto& task = tasks[i];
		std::vector<Node*> path = task.pathFuture.get();

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
		
		// Path returned by AStarSearchThreadSafe is already goal -> start reversed
		entry.path.assign(path.rbegin(), path.rend());
		entry.pathIndex = 0;
		entry.moveTimer = 0.0f;

		mAgents.push_back(std::move(entry));
	}
}

//  copy oa the A* implementation from AStarSearch but with local state to be thread safe

std::vector<Node*> Ai::AStarSearchThreadSafe(Node* start, Node* goal) const
{
	std::vector<Node*> path;
	
	// local state array instead of copying the grid
	struct SearchState {
		float gCost = FLT_MAX;
		float hCost = FLT_MAX;
		float fCost = FLT_MAX;
		Node* parent = nullptr;
		bool visited = false;
	};

	std::unique_ptr<SearchState[]> states = std::make_unique<SearchState[]>(ROW_COUNT * COL_COUNT);
	
	auto GetState = [&states](int r, int c) -> SearchState& {
		return states[r * COL_COUNT + c];
	};

	auto Heuristic = [](Node* a, Node* b) {
		return std::abs(a->row - b->row) + std::abs(a->col - b->col);
	};

	auto Compare = [&GetState](Node* a, Node* b) {
		SearchState& stateA = GetState(a->row, a->col);
		SearchState& stateB = GetState(b->row, b->col);
		return stateA.fCost > stateB.fCost || (stateA.fCost == stateB.fCost && stateA.hCost > stateB.hCost); 
	};

	std::priority_queue<Node*, std::vector<Node*>, decltype(Compare)> openSet(Compare);

	SearchState& startState = GetState(start->row, start->col);
	startState.gCost = 0.0f;
	startState.hCost = Heuristic(start, goal);
	startState.fCost = startState.hCost;

	openSet.push(start);

	while (!openSet.empty())
	{
		Node* current = openSet.top();
		openSet.pop();
		
		SearchState& currentState = GetState(current->row, current->col);
		currentState.visited = true;

		if (current == goal)
		{
			Node* curr = current;
			while (curr != nullptr)
			{
				path.push_back(curr);
				curr = GetState(curr->row, curr->col).parent;
			}
			return path;
		}

		Node* neighbors[4] = { nullptr, nullptr, nullptr, nullptr };
		int ncount = 0;
		int r = current->row;
		int c = current->col;
		
		if (r - 1 >= 0) neighbors[ncount++] = const_cast<Node*>(&grid[r - 1][c]);
		if (r + 1 < ROW_COUNT) neighbors[ncount++] = const_cast<Node*>(&grid[r + 1][c]);
		if (c + 1 < COL_COUNT) neighbors[ncount++] = const_cast<Node*>(&grid[r][c + 1]);
		if (c - 1 >= 0) neighbors[ncount++] = const_cast<Node*>(&grid[r][c - 1]);

		for (int i = 0; i < ncount; ++i)
		{
			Node* neighbor = neighbors[i];
			SearchState& neighborState = GetState(neighbor->row, neighbor->col);

			if (neighbor->blocked || neighborState.visited)
				continue;

			float tentative_gScore = currentState.gCost + 1.0f;
			if (tentative_gScore < neighborState.gCost)
			{
				neighborState.gCost = tentative_gScore;
				neighborState.hCost = Heuristic(neighbor, goal);
				neighborState.fCost = neighborState.gCost + neighborState.hCost;
				neighborState.parent = current;
				openSet.push(neighbor);
			}
		}
	}
	
	return path;
}


void Ai::UpdateAgents(float deltaTime)
{
	for (auto& agent : mAgents)
	{
		if (agent.path.empty())
			continue;

		// Finished the whole path?
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
			// Snap to waypoint and advance
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
	// A small palette of colours so agents are distinguishable
	static const SpEngine::Graphics::Color kPalette[] =
	{
		{ 0.2f, 0.8f, 1.0f, 1.0f },   // cyan
		{ 1.0f, 0.5f, 0.0f, 1.0f },   // orange
		{ 0.4f, 1.0f, 0.4f, 1.0f },   // green
		{ 1.0f, 0.2f, 0.8f, 1.0f },   // magenta
		{ 1.0f, 1.0f, 0.2f, 1.0f },   // yellow
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