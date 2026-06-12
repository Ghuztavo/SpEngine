#pragma once

#include <SpEngine/Inc/SpEngine.h>
#include "Node.h"

class Ai
{
public:
#define ROW_COUNT 100
#define COL_COUNT 100
#define NODE_SIZE 1.0f
#define AGENT_COUNT 50
#define AGENT_MOVE_SPEED 4.0f
#define OBSTACLE_COUNT 100

	// Grid
	std::vector<std::vector<Node>> grid;

	void Start(SpEngine::GameWorld& world);
	void SpawnObstacles(SpEngine::GameWorld& world);
	void DrawGrid();

	// helpers
	Node* GetRandomNode();
	std::vector<Node*> GetNeighbor(Node* node);
	//float Heuristic(Node* a, Node* b);
	void AStarSearch(Node* start, Node* goal);
	std::vector<Node*> AStarSearchThreadSafe(Node* start, Node* goal) const;

	// agents data
	struct AgentEntry
	{
		SpEngine::GameObject*   gameObject  = nullptr;
		Node*                   startNode   = nullptr;
		Node*                   goalNode    = nullptr;
		std::vector<Node*>      path;
		int                     pathIndex   = 0;
		float                   moveTimer   = 0.0f;
	};

	void SpawnAgentsSingleThreaded(SpEngine::GameWorld& world);
	void SpawnAgentsMultiThreaded(SpEngine::GameWorld& world);
	void UpdateAgents(float deltaTime);
	void DrawAgentPaths();

private:
	std::vector<AgentEntry> mAgents;

	// Converts a grid node's (row,col) to a centred world XZ position
	SpEngine::Math::Vector3 NodeToWorld(const Node* node) const;

	std::vector<Node*> aStarPath;
};